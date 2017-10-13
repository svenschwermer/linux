/*
 * Intel Merrifield FLIS platform device initialization file
 *
 * Copyright (C) 2016, Intel Corporation
 *
 * Author: Andy Shevchenko <andriy.shevchenko@linux.intel.com>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; version 2
 * of the License.
 */

#include <linux/init.h>
#include <linux/ioport.h>
#include <linux/platform_device.h>
#include <linux/pinctrl/machine.h>
#include <linux/moduleparam.h>

#include <asm/intel-mid.h>

#define FLIS_BASE_ADDR			0xff0c0000
#define FLIS_LENGTH			0x8000

static bool pinctrl_i2c6 = true;

static struct resource mrfld_pinctrl_mmio_resource = {
	.start		= FLIS_BASE_ADDR,
	.end		= FLIS_BASE_ADDR + FLIS_LENGTH - 1,
	.flags		= IORESOURCE_MEM,
};

static struct platform_device mrfld_pinctrl_device = {
	.name		= "pinctrl-merrifield",
	.id		= PLATFORM_DEVID_NONE,
	.resource	= &mrfld_pinctrl_mmio_resource,
	.num_resources	= 1,
};

static const struct pinctrl_map mapping[] __initconst = {
	{
		.dev_name = "0000:00:09.1",
		.name = PINCTRL_STATE_DEFAULT,
		.type = PIN_MAP_TYPE_MUX_GROUP,
		.ctrl_dev_name = "pinctrl-merrifield",
		.data.mux.group = "i2c6_grp",
		.data.mux.function = "i2c6",
	},
};

static int __init mrfld_pinctrl_init(void)
{
	int ret;

	if (intel_mid_identify_cpu() != INTEL_MID_CPU_CHIP_TANGIER)
		return -ENODEV;

	ret = platform_device_register(&mrfld_pinctrl_device);
	if (ret)
		return ret;

	if (pinctrl_i2c6) {
		ret = pinctrl_register_mappings(mapping, ARRAY_SIZE(mapping));
		if (ret)
			pr_err("Failed to register i2c6 pinctrl mapping\n");
	}

	return 0;
}
arch_initcall(mrfld_pinctrl_init);

module_param(pinctrl_i2c6, bool, 0444);
MODULE_PARM_DESC(pinctrl_i2c6, "Configure pinctrl to use i2c-6");
