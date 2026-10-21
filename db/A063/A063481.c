/*
 * A063481  a(n) = 4^n + 8^n.
 * https://oeis.org/A063481
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 4^n + 8^n
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 21, u128 n <= 42,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t v4 = 1;   /* 4^n */
    seq_t v8 = 1;   /* 8^n */
    idx_t i;

    for (i = 0; i < n; ++i) {
        v4 *= 4;
        v8 *= 8;
    }

    return v4 + v8;
}
