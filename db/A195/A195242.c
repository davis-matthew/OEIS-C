/*
 * A195242  Expansion of Sum_{n>=0} n^n*x^n/(1 - n*x)^n.
 * https://oeis.org/A195242
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} C(n-1,k)*(k+1)^n (OEIS A195242)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 14, u128 n <= 25,
 *            big any n
 * Helpers:   seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/seq_pow.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;

    seq_t result = (seq_t)0;
    /* C(n-1,0) = 1 */
    seq_t binom = (seq_t)1;

    for (idx_t k = 0; k <= n; ++k)
    {
        /* add C(n-1,k)*(k+1)^n */
        seq_t pow_term = seq_pow((seq_t)(k + 1), n);
        result += binom * pow_term;

        /* update binom to C(n-1,k+1) for next iteration \
           (no need after last step, but harmless) */
        if (k < n)
        {
            /* C(n-1,k+1) = C(n-1,k) * (n-1-k) / (k+1) */
            seq_t numerator   = (seq_t)(n - 1 - k);
            seq_t denominator = (seq_t)(k + 1);
            binom = binom * numerator / denominator;
        }
    }

    return result;
}
