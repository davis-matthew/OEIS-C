/*
 * A036464  Number of ways to place two nonattacking queens on an n X n board.
 * https://oeis.org/A036464
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(n) = binomial(n,3)*(3*n-1)
 * Range:     u8 n <= 5, u16 n <= 19, u32 n <= 305, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    if (n < 3)
        return 0;
    seq_t bin = binomial(n, 3);
    seq_t mult = (seq_t)(3 * (seq_t)n - 1);
    return bin * mult;
}
