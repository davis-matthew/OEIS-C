/*
 * A035301  Expansion of Sum_{n>=0} (q^n / Product_{k=1..n+5} (1 - q^k)).
 * https://oeis.org/A035301
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{k=0}^n p_{n+5-k}(k) (coefficient of q^n in Σ_{m≥0} q^m
 *            / ∏_{k=1}^{m+5} (1–q^k))
 * Range:     u8 n <= 12, u16 n <= 38, u32 n <= 44, u64 n <= 44, u128 n <= 44,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *a_cache = NULL;
static idx_t cache_max = 0;

static void recompute(idx_t nmax)
{
    seq_t *dp = (seq_t *)malloc((nmax + 1) * sizeof(seq_t));
    seq_t *a_arr = (seq_t *)malloc((nmax + 1) * sizeof(seq_t));
    if (!dp || !a_arr) {
        free(dp);
        free(a_arr);
        return;
    }

    for (idx_t i = 0; i <= nmax; ++i) {
        dp[i] = 0;
        a_arr[i] = 0;
    }
    dp[0] = 1;   /* partitions of 0 */

    for (idx_t k = 1; k <= nmax + 5; ++k) {
        if (k <= nmax) {
            for (idx_t m = k; m <= nmax; ++m) {
                dp[m] += dp[m - k];
            }
        }

        if (k >= 5) {
            idx_t offset = k - 5;
            if (offset > nmax) break;
            for (idx_t t = 0; t + offset <= nmax; ++t) {
                a_arr[t + offset] += dp[t];
            }
        }
    }

    free(a_cache);
    a_cache = a_arr;
    cache_max = nmax;

    free(dp);
}

seq_t a(idx_t n)
{
    if (a_cache == NULL || n > cache_max) {
        recompute(n);
        if (a_cache == NULL) return 0; /* allocation failure */
    }
    return a_cache[n];
}
