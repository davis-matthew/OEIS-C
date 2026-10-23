/*
 * A022653  Expansion of Product_{m>=1} (1+m*q^m)^25.
 * https://oeis.org/A022653
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = coefficient of q^n in ∏_{m≥1} (1 + m*q^m)^25
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 10, u64 n <= 27, u128 n <= 75,
 *            big any n
 * Helpers:   alloc_seq_array, free_seq_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation uses dynamic programming over the generating
   function. The auxiliary functions alloc_seq_array and free_seq_array
   handle allocation of seq_t tables in a manner that works for both
   integer and arbitrary‑precision builds. */

#include <stdbool.h>
#include "../utils/alloc_seq_array.h"
#include "../utils/free_seq_array.h"

seq_t a(idx_t n)
{
    /* Base case */
    if (n == 0)
        return (seq_t)1;

    /* Pre‑compute binomial coefficients C(25,k)  (k = 0..25)   */
    static seq_t binom[26];
    static bool binom_initialized = false;
    if (!binom_initialized) {
        binom[0] = (seq_t)1;
        for (idx_t k = 1; k <= 25; ++k) {
            /* binom[k] = binom[k-1] * (25 - k + 1) / k;  */
            binom[k] = binom[k - 1] * ((seq_t)(25 - k + 1)) / (seq_t)k;
        }
        binom_initialized = true;
    }

    /* Allocate convolution tables.  alloc_seq_array handles
       proper construction for arbitrary‑precision seq_t. */
    seq_t *dp     = alloc_seq_array(n + 1);
    seq_t *newdp  = alloc_seq_array(n + 1);

    /* Initialise to zero */
    for (idx_t i = 0; i <= n; ++i) {
        dp[i]    = (seq_t)0;
        newdp[i] = (seq_t)0;
    }
    dp[0] = (seq_t)1; /* a(0) = 1 */

    /* Dynamic programming over parts m = 1 .. n */
    for (idx_t m = 1; m <= n; ++m) {
        /* Determine maximum k such that m*k ≤ n and k ≤ 25   */
        idx_t maxk = 25;
        if (m != 0) {
            idx_t t = n / m;
            if (t < maxk)
                maxk = t;
        }

        /* Pre‑compute weights w[k] = C(25,k) * m^k  for k = 0..maxk   */
        seq_t w[26];
        w[0] = (seq_t)1;
        seq_t pow_m = (seq_t)1;
        for (idx_t k = 1; k <= maxk; ++k) {
            pow_m *= (seq_t)m;          /* m^k   */
            w[k] = binom[k] * pow_m;    /* weight */
        }

        /* Copy current polynomial into newdp */
        for (idx_t j = 0; j <= n; ++j)
            newdp[j] = dp[j];

        /* Add contributions of higher powers of (1 + m*q^m)^25   */
        for (idx_t k = 1; k <= maxk; ++k) {
            idx_t shift = m * k;             /* exponent shift */
            seq_t wk   = w[k];
            for (idx_t j = shift; j <= n; ++j) {
                newdp[j] = newdp[j] + dp[j - shift] * wk;
            }
        }

        /* Swap tables for next iteration   */
        seq_t *tmp = dp;
        dp = newdp;
        newdp = tmp;
    }

    seq_t result = dp[n];

    /* Free allocated tables   */
    free_seq_array(dp);
    free_seq_array(newdp);

    return result;
}
