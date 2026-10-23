/*
 * A008712  15 in base 15-n.
 * https://oeis.org/A008712
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  fini, nonn, base, full
 * Formula:   a(n) = representation of 15 in base (15 - n)
 * Range:     u8 n <= 12, u16 n <= 13, u32 n <= 13, u64 n <= 14, u128 n <= 14,
 *            big any n
 * Helpers:   seq_from_str
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "../utils/seq_from_str.h"

seq_t a(idx_t n)
{
    /* compute the base */
    int64_t base = (int64_t)15 - (int64_t)n;

    if (base == 0) {
        /* base 0 is undefined – return 0 as a sentinel */
        return 0;
    }

    if (base == 1) {            /* unary representation of 15 */
        char buf[20];
        for (int i = 0; i < 15; ++i) buf[i] = '1';
        buf[15] = '\0';
        return seq_from_str(buf);
    }

    if (base == -1) {           /* base -1 cannot represent 15 */
        return 0;
    }

    int64_t abs_base = base > 0 ? base : -base;

    /* digits of the representation (least significant first) */
    int digits[64];
    size_t dcnt = 0;
    int64_t num = 15;

    if (base > 0) {              /* ordinary positive base */
        while (num > 0) {
            int64_t r = num % base;
            digits[dcnt++] = (int)r;
            num /= base;
        }
    } else { /* negative base */
        while (num != 0) {
            int64_t r = num % abs_base;
            if (r < 0) r += abs_base;
            digits[dcnt++] = (int)r;
            num = (num - r) / base; /* base is negative */
        }
    }

    /* build the decimal string corresponding to the digit sequence */
    char buf[256];
    size_t pos = 0;
    for (size_t i = dcnt; i-- > 0; ) {
        int len = snprintf(buf + pos, sizeof(buf) - pos, "%u", (unsigned)digits[i]);
        pos += len;
    }
    buf[pos] = '\0';
    return seq_from_str(buf);
}
