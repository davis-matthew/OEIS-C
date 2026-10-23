/*
 * A028270  Central elements in 3-Pascal triangle A028262 (by row).
 * https://oeis.org/A028270
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = binomial(2*n, n) + binomial(2*n-2, n-1)
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;

    seq_t part1 = binomial(2 * n, n);
    seq_t part2 = binomial(2 * n - 2, n - 1);

    return part1 + part2;
}
