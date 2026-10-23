/*
 * A033803  Product t2(q^d); d | 44, where t2 = theta2(q)/(2*q^(1/4)).
 * https://oeis.org/A033803
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = coefficient of q^n in ∏_{d|44} ( ∑_{k≥0} q^{d·k·(k+1)/2}
 *            ) where the divisors d of 44 are 1, 2, 4, 11, 22, 44. The
 *            coefficient counts the number of solutions to n = Σ_{d|44}
 *            d·k_d·(k_d+1)/2 with k_d ≥ 0
 * Range:     u8 n <= 176, u16 n <= 2861, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   alloc_seq_array, free_seq_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>        /* for size_t */

/* the dedicated allocation/free helpers are assumed to exist.
   They are part of the build system. */
#include "../utils/alloc_seq_array.h"   /* seq_t *alloc_seq_array(size_t); */
#include "../utils/free_seq_array.h"    /* void free_seq_array(seq_t *); */

seq_t a(idx_t n)
{
    /* Number of coefficients to store.  n is idx_t (unsigned),
       cast to size_t safely for array indexing. */
    const size_t Nsize = (size_t)n + 1;

    /* Allocate two arrays for dynamic programming. */
    seq_t *dp     = alloc_seq_array(Nsize);
    seq_t *newdp  = alloc_seq_array(Nsize);

    /* initialise dp[0] = 1, rest 0 */
    for (size_t i = 0; i < Nsize; ++i) dp[i] = 0;
    dp[0] = 1;

    /* divisors of 44 */
    static const idx_t divisors[6] = { 1, 2, 4, 11, 22, 44 };
    const size_t divcnt = sizeof(divisors) / sizeof(divisors[0]);

    for (size_t di = 0; di < divcnt; ++di) {
        idx_t d = divisors[di];

        /* clear newdp */
        for (size_t i = 0; i < Nsize; ++i) newdp[i] = 0;

        /* convolution with the series for this d */
        for (size_t ii = 0; ii < Nsize; ++ii) {
            seq_t val = dp[ii];
            if (!val) continue;

            /* iterate over non‑negative k */
            for (idx_t k = 0; ; ++k) {
                /* triangular number times d */
                idx_t w = d * k * (k + 1) / 2;
                if (w > n - (idx_t)ii) break;

                size_t target = ii + (size_t)w;
                newdp[target] = newdp[target] + val;
            }
        }

        /* swap dp and newdp for next divisor */
        seq_t *tmp = dp;
        dp = newdp;
        newdp = tmp;
    }

    /* result is coefficient of q^n */
    seq_t result = dp[n];

    /* free allocated memory */
    free_seq_array(dp);
    free_seq_array(newdp);

    return result;
}
