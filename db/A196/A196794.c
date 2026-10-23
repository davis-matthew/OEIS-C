/*
 * A196794  a(n) = Sum_{k=0..n} binomial(n,k)*2^k*(k+1)^(n-k).
 * https://oeis.org/A196794
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} binomial(n,k)*2^k*(k+1)^(n-k)
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 11, u64 n <= 19, u128 n <= 34,
 *            big any n
 * Helpers:   pow2, pow_int
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"
#include "../utils/pow_int.h"

seq_t a(idx_t n)
{
    seq_t result = 0;
    seq_t binom   = 1;

    for (idx_t k = 0; k <= n; ++k)
    {
        seq_t term = binom;                         /* binomial(n,k)        */
        term *= pow2(k);                            /* * 2^k                */
        term *= pow_int((seq_t)(k + 1), n - k);     /* * (k+1)^(n-k)        */
        result += term;                             /* accumulate           */

        if (k < n)                                  /* prepare next binom   */
            binom = binom * (n - k) / (k + 1);
    }

    return result;
}
