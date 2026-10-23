/*
 * A129504  Row sums of triangle A129503.
 * https://oeis.org/A129504
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{k≥0 , 2^k ≤ n} binomial(n - 2^k + k , k)
 * Range:     u8 n <= 15, u16 n <= 45, u32 n <= 176, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation computes the binomial coefficient in a
   way that avoids overflow on 64‑bit values by using a 128‑bit
   intermediate for the multiplicative calculation. */
#include <stdint.h>
#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    if (n == 0)   /* sequence is undefined for n=0; return 0 */
        return 0;

    seq_t sum = 0;

    for (idx_t k = 0; ( (idx_t)1 << k ) <= n; ++k)
    {
        idx_t pow2k = (idx_t)1 << k;

        /* n - 2^k + k  (this expression is guaranteed to fit in idx_t) */
        idx_t nm = (idx_t)n - pow2k + k;

        /* Compute binomial(nm, k) multiplicatively using a 128‑bit
           intermediate to avoid overflow when seq_t is 64‑bit.  */
        unsigned __int128 bin = 1;
        for (idx_t i = 1; i <= k; ++i)
        {
            unsigned __int128 term = (unsigned __int128)(nm - (i - 1));
            bin = bin * term;
            bin /= i;
        }
        sum += (seq_t)bin;
    }
    return sum;
}
