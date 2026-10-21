/*
 * A218496  4th iteration of the hyperbinomial transform on the sequence of
 *          1's.
 * https://oeis.org/A218496
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} 4 * (k+4)^(k-1) * C(n,k)
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 8, u64 n <= 15, u128 n <= 26,
 *            big any n
 * Helpers:   big_pow, binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/big_pow.h"
#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    seq_t total = 0;
    for (idx_t k = 0; k <= n; ++k)
    {
        seq_t bin = binomial(n, k);
        seq_t term;
        if (k == 0)        /* 4 * 4^-1 = 1 */
        {
            term = bin;    /* binomial(n,0) = 1 */
        }
        else
        {
            seq_t pow = big_pow((seq_t)(k + 4), (seq_t)(k - 1));
            term = pow * bin;
            term *= (seq_t)4;
        }
        total += term;
    }
    return total;
}
