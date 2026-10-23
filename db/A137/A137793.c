/*
 * A137793  Number of partitions of n into distinct parts with no prime gaps
 *          in their factorization.
 * https://oeis.org/A137793
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of partitions of n into distinct parts with no
 *            prime gaps in their factorization
 * Range:     u8 n <= 32, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation computes the required numbers by dynamic programming
   over the set of integers that are "gap‑free".  An integer m is gap‑free
   if all distinct primes dividing m form a consecutive block of primes.
   The dp array stores the number of ways to obtain a given sum using each
   valid integer at most once (distinct parts).  The routine remembers
   the largest n that has been computed so far and expands the table only
   when a larger argument is requested.  It is safe to call it with
   n in any order. */

#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    static idx_t max_n = 0;     /* largest n processed so far       */
    static seq_t *dp = NULL;    /* dp table: dp[i] = number of ways to sum to i */

    if (n == 0) return 0; /* undefined domain but harmless */

    /* If a larger n is requested, rebuild everything from scratch */
    if (n > max_n) {
        /* Free previous dp if it exists */
        if (dp != NULL) {
            free(dp);
            dp = NULL;
        }

        /* Allocate dp array */
        dp = (seq_t *)malloc((n + 1) * sizeof(seq_t));
        if (dp == NULL) return 0; /* allocation failure (should not happen) */

        /* Base case */
        dp[0] = 1;
        for (idx_t i = 1; i <= n; ++i) dp[i] = 0;

        /* -----------------------------------------------------------------
           Compute smallest prime factor (spf) array up to n
           ----------------------------------------------------------------- */
        idx_t *spf = (idx_t *)malloc((n + 1) * sizeof(idx_t));
        for (idx_t i = 2; i <= n; ++i) spf[i] = i;
        for (idx_t i = 2; i * i <= n; ++i) {
            if (spf[i] == i) {
                for (idx_t j = i * i; j <= n; j += i) {
                    if (spf[j] == j) spf[j] = i;
                }
            }
        }
        spf[1] = 1;  /* sentinel */

        /* -----------------------------------------------------------------
           Gather list of primes up to n
           ----------------------------------------------------------------- */
        size_t aprox = n / 2 + 1;          /* upper bound for prime count */
        idx_t *primes = (idx_t *)malloc(aprox * sizeof(idx_t));
        size_t prime_cnt = 0;
        for (idx_t i = 2; i <= n; ++i) {
            if (spf[i] == i) {
                primes[prime_cnt++] = i;
            }
        }

        /* -----------------------------------------------------------------
           Build mapping from prime value to its order among primes
           ----------------------------------------------------------------- */
        idx_t *prime_index = (idx_t *)calloc((size_t)(n + 1), sizeof(idx_t));
        for (size_t i = 0; i < prime_cnt; ++i) {
            prime_index[primes[i]] = (idx_t)(i + 1);  /* 1‑based for zero check */
        }

        /* -----------------------------------------------------------------
           Dynamic programming over all integers 1 … n
           ----------------------------------------------------------------- */
        for (idx_t m = 1; m <= n; ++m) {
            bool valid = true;

            if (m > 1) {
                idx_t temp = m;
                idx_t prev_idx = 0;
                while (temp > 1) {
                    idx_t p = spf[temp];
                    /* skip repeated prime factor occurrences */
                    while (temp % p == 0) temp /= p;
                    idx_t cur_idx = prime_index[p];
                    if (prev_idx != 0 && cur_idx != prev_idx + 1) {
                        valid = false;
                        break;
                    }
                    prev_idx = cur_idx;
                }
            }   /* m == 1 is trivially valid */

            if (valid) {
                for (idx_t j = n; j >= m; --j) {
                    dp[j] += dp[j - m];
                }
            }
        }

        /* Clean up temporary arrays */
        free(spf);
        free(primes);
        free(prime_index);

        /* Remember the maximum n processed */
        max_n = n;
    }

    /* n is within [1, max_n] now, return the cached answer */
    return dp[n];
}
