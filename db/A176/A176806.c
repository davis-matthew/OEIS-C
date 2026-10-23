/*
 * A176806  Consider asymmetric 1-D random walk with set of possible jumps
 *          {-1,+1,+2}. Sequence gives number of paths of length n ending at
 *          origin.
 * https://oeis.org/A176806
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{k=⌊(n+2)/3⌋ .. ⌊n/2⌋} binomial(n,k) * binomial(k,
 *            3*k-n)
 * Range:     u8 n <= 7, u16 n <= 14, u32 n <= 25, u64 n <= 31, u128 n <= 31,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/binom.h"

seq_t a(idx_t n)
{
    /* Determine summation bounds */
    idx_t k_min = (n + 2) / 3;
    idx_t k_max = n / 2;

    seq_t sum = (seq_t)0;
    for (idx_t k = k_min; k <= k_max; ++k)
    {
        seq_t term1 = binom(n, k);
        idx_t inner = 3 * k - n;
        seq_t term2 = binom(k, inner);
        sum += term1 * term2;
    }
    return sum;
}
