/*
 * A075045  Coefficients A_n for the s=3 tennis ball problem.
 * https://oeis.org/A075045
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} 2^(n-k) * binomial(3*n+4, k)
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 21, u128 n <= 21,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Uses the binomial utility in utils. */

#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    /* Compute 2^n first */
    seq_t pow2 = 1;
    for (idx_t i = 0; i < n; ++i)
        pow2 <<= 1;

    seq_t result = 0;
    for (idx_t k = 0; k <= n; ++k)
    {
        seq_t bin = binomial((3*n + 4), k);
        result += pow2 * bin;
        if (k == n) break;        /* last iteration */
        pow2 >>= 1;                /* next lower power of two */
    }
    return result;
}
