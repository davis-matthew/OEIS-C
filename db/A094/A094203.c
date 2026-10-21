/*
 * A094203  a(n) = 24 written in base 13-n.
 * https://oeis.org/A094203
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = representation of 24 in base (13 - n), interpreted as a
 *            decimal integer. * * For n = 1 … 12, the base (13 - n) runs from
 *            12 down to 1. * Base 1 is handled as the unary representation: a
 *            string of 24 '1' digits. * For bases >= 2, the usual positional
 *            conversion of 24 to base b is performed,...
 * Range:     u8 n <= 10, u16 n <= 11, u32 n <= 11, u64 n <= 11, u128 n <= 12,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>   /* for size_t, NULL */

seq_t a(idx_t n)
{
    /* n is 1‑based.  We only need to support n >= 1. */
    if (n == 0) return (seq_t)0;   /* safety for undefined offset */

    /* Compute the target base (13 - n). */
    idx_t base = (idx_t)13 - n;

    /* Unary representation when base <= 1, i.e. base == 1 for n == 12. */
    if (base <= 1) {
        seq_t res = (seq_t)0;
        for (idx_t i = 0; i < 24; ++i)
            res = res * (seq_t)10 + (seq_t)1;
        return res;
    }

    /* Standard conversion for bases 2 … 12.  24 fits comfortably into idx_t. */
    idx_t temp = (idx_t)24;
    int digits[32];  /* 24 in base 2 requires at most 5 digits, so this is plenty. */
    idx_t cnt = 0;

    while (temp > 0) {
        digits[cnt++] = (int)(temp % base);
        temp /= base;
    }
    /* digits[0] is least significant.  Build the decimal number from most
       significant to least significant. */
    seq_t res = (seq_t)0;
    while (cnt > 0) {
        --cnt;
        res = res * (seq_t)10 + (seq_t)digits[cnt];
    }

    return res;
}
