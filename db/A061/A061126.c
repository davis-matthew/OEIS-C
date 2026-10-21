/*
 * A061126  Number of degree-n permutations of order exactly 16.
 * https://oeis.org/A061126
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = number of permutations of n with order exactly 16. The
 *            recurrence is based on the cycle‑length set {1,2,4,8,16}. For
 *            each permutation on n elements we pick the cycle containing
 *            element 1. Let L be its length (L∈{1,2,4,8,16}). The number of
 *            ways to choose such a cycle is (n‑1)!/(n‑L)!. The...
 * Range:     u8 n <= 15, u16 n <= 15, u32 n <= 15, u64 n <= 21, u128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <string.h>

static idx_t maxComputed = 0;
static idx_t dpCap = 0;
static seq_t *dp = NULL;         /* flattened 2‑D array dp[n*5 + k] */

static void ensure_array(idx_t size)
{
    if (size <= maxComputed) return;

    idx_t newCap = size + 1;   /* also store dp[0] */
    if (dp == NULL) {
        dp = (seq_t*)calloc(newCap * 5, sizeof(seq_t));
        dpCap = newCap;
    } else if (newCap > dpCap) {
        dp = (seq_t*)realloc(dp, newCap * 5 * sizeof(seq_t));
        /* zero new entries */
        memset(dp + dpCap * 5, 0, (newCap - dpCap) * 5 * sizeof(seq_t));
        dpCap = newCap;
    }

    /* base case dp[0] */
    if (maxComputed == 0) {
        dp[0] = 1;            /* dp[0*5 + 0] */
        for (int k = 1; k < 5; ++k) dp[0*5 + k] = 0;
    }

    const int S[5]   = {1, 2, 4, 8, 16};
    const int expS[5] = {0, 1, 2, 3, 4};

    for (idx_t n = maxComputed + 1; n <= size; ++n) {
        /* dp[n][k] defaults already 0 from calloc/realloc */
        for (int s = 0; s < 5; ++s) {
            int L   = S[s];
            int t   = expS[s];
            if ((idx_t)L > n) continue;

            /* compute factor = (n-1)! / (n-L)!  = product_{i=n-L+1}^{n-1} i */
            seq_t factor = 1;
            for (idx_t ii = n - (idx_t)L + 1; ii <= n - 1; ++ii)
                factor *= (seq_t)ii;

            idx_t base = (n - (idx_t)L);
            for (int kRest = 0; kRest < 5; ++kRest) {
                seq_t rest = dp[base*5 + kRest];
                if (rest == 0) continue;
                int k = (t > kRest) ? t : kRest;
                dp[n*5 + k] += factor * rest;
            }
        }
    }
    maxComputed = size;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined, but harmless */
    ensure_array(n);
    return dp[n*5 + 4];            /* lcm = 2^4 = 16 */
}
