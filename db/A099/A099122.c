/*
 * A099122  Number of orbits of the wreath product of S_n with S_n on n X n
 *          matrices over {0,1,2,3}.
 * https://oeis.org/A099122
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = binomial(binomial(n+3, n) + n - 1, n)
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 6, u64 n <= 10, u128 n <= 16,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    /* Compute k = binomial(n+3, n) + n - 1.
       binomial(n+3, n) = C(n+3,3) = (n+3)(n+2)(n+1)/6 */
    idx_t k = ((idx_t)n + 3) * ((idx_t)n + 2) * ((idx_t)n + 1) / 6;
    k += (idx_t)n - 1;
    /* Return C(k, n).  binomial returns a value of type seq_t. */
    return binomial(k, n);
}
