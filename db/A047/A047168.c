/*
 * A047168  Number of nonempty subsets of {1,2,...,n} in which exactly 4/5 of
 *          the elements are <= n/2.
 * https://oeis.org/A047168
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{k≥1} binomial(⌊n/2⌋,4k)·binomial(⌈n/2⌉,k) (from OEIS
 *            A047168)
 * Range:     u8 n <= 14, u16 n <= 25, u32 n <= 45, u64 n <= 87, u128 n <= 169,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    /* floor(n/2) and ceil(n/2) */
    idx_t half_floor = n / 2;
    idx_t half_ceil  = (n + 1) / 2;

    if (half_floor < 4)  /* no possible 4k term */
        return (seq_t)0;

    seq_t sum = (seq_t)0;

    idx_t maxk = half_floor / 4;
    for (idx_t k = 1; k <= maxk && k <= half_ceil; ++k)
    {
        seq_t t1 = binomial(half_floor, 4 * k);
        seq_t t2 = binomial(half_ceil, k);
        sum += t1 * t2;
    }
    return sum;
}
