/*
 * A095513  a(n) = 60 written in base 15 - n.
 * https://oeis.org/A095513
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   a(n) = decimal representation of 60 in base (15 - n)
 * Range:     u8 n <= 10, u16 n <= 12, u32 n <= 13, u64 n <= 13, u128 n <= 13,
 *            big any n
 * Helpers:   seq_from_str
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/seq_from_str.h"

seq_t a(idx_t n)
{
    /* The sequence is defined for n = 0 … 14 only (base >= 1). */
    if (n > 14) {
        /* Behavior for n > 14 is undefined; return zero. */
        return 0;
    }

    idx_t base = 15 - n;          /* base is between 15 and 1 inclusive */
    static char buf[65];          /* at most 60 digits for base 1, plus NUL */

    if (base == 1) {
        /* Unary representation: 60 ones */
        for (idx_t i = 0; i < 60; ++i)
            buf[i] = '1';
        buf[60] = '\0';
        return seq_from_str(buf);
    }

    /* For base >= 2: convert 60 to that base */
    idx_t tmp = 60;
    idx_t digits[10];             /* sufficient for 60 in any base <=15 */
    int dcnt = 0;
    while (tmp > 0) {
        digits[dcnt++] = tmp % base;
        tmp /= base;
    }

    int pos = 0;
    for (int i = dcnt - 1; i >= 0; --i) {
        idx_t d = digits[i];
        if (d < 10) {
            buf[pos++] = '0' + (char)d;
        } else {
            /* base <= 15, thus d is at most 14 (two decimal digits) */
            buf[pos++] = '1';
            buf[pos++] = '0' + (char)(d - 10);
        }
    }
    buf[pos] = '\0';
    return seq_from_str(buf);
}
