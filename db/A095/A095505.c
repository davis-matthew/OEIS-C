/*
 * A095505  a(n) = 56 written in base 14 - n.
 * https://oeis.org/A095505
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   a(n) = representation of 56 in base (14 - n) interpreted as a
 *            decimal integer. For base 1 the representation is 56 ones; for
 *            base <= 0 a(n) is defined as 0
 * Range:     u8 n <= 9, u16 n <= 11, u32 n <= 12, u64 n <= 12, u128 n <= 12,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    /* Compute the base b = 14 - n */
    idx_t b = 14u - n;

    /* Base <= 0 is not defined – return 0 */
    if (b == 0 || n > 13u) {
        return (seq_t)0U;
    }

    /* Base 1: 56 is written as 56 ones */
    if (b == 1u) {
        seq_t res = 0U;
        for (size_t i = 0; i < 56; ++i) {
            res = res * 10 + 1;
        }
        return res;
    }

    /* For bases >= 2, convert 56 into base b */
    uint64_t val = 56U;                 /* 56 is small enough for uint64_t */
    int digits[64];
    int len = 0;
    while (val > 0) {
        digits[len++] = (int)(val % b);
        val /= b;
    }

    /* Build the decimal integer from the collected digits */
    seq_t res = 0U;
    for (int i = len - 1; i >= 0; --i) {
        res = res * 10 + digits[i];
    }

    return res;
}
