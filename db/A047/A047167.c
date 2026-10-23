/*
 * A047167  Number of nonempty subsets of {1,2,...,n} in which exactly 3/5 of
 *          the elements are <= n/2.
 * https://oeis.org/A047167
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k>=1} binomial(floor(n/2), 3*k) *
 *            binomial(ceil(n/2), 2*k)
 * Range:     u8 n <= 11, u16 n <= 20, u32 n <= 37, u64 n <= 70, u128 n <= 137,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* not needed for OEIS offset, but safe */

    idx_t half_floor  = n / 2;                /* floor(n/2)   */
    idx_t half_ceil   = (n + 1) / 2;          /* ceil(n/2)    */
    seq_t sum = 0;

    /* maximum k such that 3*k <= half_floor */
    idx_t max_k = half_floor / 3;
    for (idx_t k = 1; k <= max_k; ++k)
    {
        seq_t c1 = binomial(half_floor, 3 * k);
        seq_t c2 = binomial(half_ceil, 2 * k);
        sum += c1 * c2;
    }
    return sum;
}
