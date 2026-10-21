/*
 * A090049  Length of longest contiguous block of 0's in binary expansion of
 *          n^3.
 * https://oeis.org/A090049
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = longest contiguous block of 0's in binary expansion of
 *            n^3
 * Range:     u8 n <= 7, u16 n <= 40, u32 n <= 7556, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t cube = (seq_t)n * (seq_t)n * (seq_t)n;
    if (cube == 0)
        return 1;

    seq_t x = cube;
    seq_t max_zero = 0;
    seq_t cur = 0;

    while (x) {
        if (x & 1) {
            if (cur > max_zero)
                max_zero = cur;
            cur = 0;
        } else {
            cur++;
        }
        x >>= 1;
    }
    if (cur > max_zero)
        max_zero = cur;

    return max_zero;
}
