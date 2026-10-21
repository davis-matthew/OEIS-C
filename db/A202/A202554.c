/*
 * A202554  Number of nX2 0,1 arrays with the row and column sums
 *          nondecreasing.
 * https://oeis.org/A202554
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n+1) + Σ_{j=0}^{n-1} (n-j)·2^j + Σ_{k even, 2≤k≤n}
 *            (n-k+1)·C(k,k/2)/2 where C(k,k/2) is the binomial coefficient.
 *            This follows from counting n×2 0‑1 matrices whose row sums are
 *            nondecreasing and whose column sums satisfy col1 ≤ col2
 * Range:     u8 n <= 6, u16 n <= 14, u32 n <= 30, u64 n <= 62, u128 n <= 125,
 *            big any n
 * Helpers:   binom_half
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/binom_half.h"

/*--------------------------------------------------------------------
   seq_t a(idx_t n)
   A202554 – Number of n×2 0,1 arrays with the row and column sums
   nondecreasing, offset 1.
--------------------------------------------------------------------*/
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* not used by the OEIS sequence     */

    seq_t result = (seq_t)(n + 1);   /* (n+1) part of the formula         */
    seq_t pow = 1;                  /* 2^0                                */

    /* First summation: Σ_{j=0}^{n-1} (n-j)·2^j                            */
    for (idx_t j = 0; j < n; ++j)
    {
        result += (seq_t)(n - j) * pow;
        pow *= 2;                    /* power of 2 for next j             */
    }

    /* Second summation: Σ_{even k} (n-k+1)·C(k,k/2)/2                     */
    for (idx_t k = 2; k <= n; k += 2)
    {
        seq_t binom = binom_half(k);
        seq_t term  = (seq_t)(n - k + 1) * binom;
        term /= 2;                   /* division is exact                 */
        result += term;
    }

    return result;
}
