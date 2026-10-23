/*
 * A156194  Period 12: repeat [1, 2, 7, 1, 7, 2, 1, 1, 4, 2, 4, 1].
 * https://oeis.org/A156194
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, less
 * Formula:   a(n) = [1, 2, 7, 1, 7, 2, 1, 1, 4, 2, 4, 1][n mod 12]
 * Range:     u8 n <= 83, u16 n <= 83, u32 n <= 83, u64 n <= 83, u128 n <= 83,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    /* 12‑term period */
    static const seq_t period[12] = {
        1, 2, 7, 1, 7, 2, 1, 1, 4, 2, 4, 1
    };
    return period[(size_t)(n % 12)];
}
