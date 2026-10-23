/*
 * A146524  a(n) is the largest nonnegative integer m such that 2*n*k+1 is
 *          prime for all k where 1<=k<=m. a(n) = 0 if 2n+1 is composite.
 * https://oeis.org/A146524
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = largest nonnegative integer m such that 2*n*k+1 is prime
 *            for all 1≤k≤m; a(n)=0 if 2n+1 is composite
 * Range:     u8 n <= 64, u16 n <= 105, u32 n <= 105, u64 n <= 105,
 *            u128 n <= 105, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/is_prime_big.h"

seq_t a(idx_t n)
{
    seq_t step = (seq_t)2 * (seq_t)n;
    seq_t term = step + 1; /* 2*n*1 + 1 */

    if (!is_prime_big(term))
        return 0;

    seq_t m = 0;
    while (is_prime_big(term))
    {
        ++m;
        term += step;
    }
    return m;
}
