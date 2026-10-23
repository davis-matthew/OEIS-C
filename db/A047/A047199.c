/*
 * A047199  Number of nonempty subsets of {1,2,...,n} in which exactly 2/5 of
 *          the elements are <= n/3.
 * https://oeis.org/A047199
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 11, u16 n <= 20, u32 n <= 34, u64 n <= 34, u128 n <= 34,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: a(n) = Σ_{t=1}^{min( ⌊n/5⌋, ⌊⎣n/3⎦/2⌋, ⌊(n-⎣n/3⎦)/3⌋ )}
 *              binomial( ⎣n/3⎦ , 2t ) * binomial( n‑⎣n/3⎦ , 3t )
 * (empty sum = 0, n ≥ 1)
 */
#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    /* number of elements ≤ n/3 */
    idx_t m = n / 3;

    /* upper bounds for t derived from 2t ≤ m and 3t ≤ n‑m */
    idx_t tmax1 = m / 2;               /* from 2t ≤ m */
    idx_t tmax2 = (n - m) / 3;         /* from 3t ≤ n‑m */
    idx_t K = (tmax1 < tmax2) ? tmax1 : tmax2;

    if (K == 0)          /* no non‑empty subset satisfies the condition */
        return 0;

    seq_t result = 0;
    for (idx_t t = 1; t <= K; ++t)
    {
        seq_t part1 = binomial(m, 2 * t);
        seq_t part2 = binomial(n - m, 3 * t);
        result += part1 * part2;
    }
    return result;
}
