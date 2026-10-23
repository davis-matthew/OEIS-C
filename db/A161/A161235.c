/*
 * A161235  Number of partitions of n into Catalan numbers A000108 where every
 *          part appears at least 10 times.
 * https://oeis.org/A161235
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 41, u16 n <= 554, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   catalan
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A161235 – Number of partitions of n into Catalan numbers A000108
    where every part appears at least 10 times.
    The function uses a one‑dimensional dynamic programming array.
    For each distinct Catalan coin c the transition is
        new[s] = old[s] + Σ_{k=10}^{⌊s/c⌋} old[s-k*c],
    computed efficiently by accumulating prefix sums for each residue
    class modulo c.  */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/catalan.h"

/* external types supplied by the build */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* offset is 1 */

    /* Dynamic programming array: dp[i] = number of partitions of i
       using the already processed Catalan coins.  */
    seq_t *dp = (seq_t *)calloc((size_t)(n + 1), sizeof(seq_t));
    if (!dp) return (seq_t)0;
    dp[0] = (seq_t)1;

    /* Build increasing list of distinct Catalan numbers <= n. */
    idx_t *coins = (idx_t *)malloc((size_t)(n + 1) * sizeof(idx_t));
    if (!coins) { free(dp); return (seq_t)0; }
    size_t coin_cnt = 0;
    for (idx_t k = 0; ; ++k) {
        seq_t c = catalan(k);            /* Catalan(k) */
        if (c > n) break;                /* larger coins are irrelevant */
        idx_t v = (idx_t)c;
        if (v == 0) continue;            /* skip accidental zero */
        if (coin_cnt == 0 || coins[coin_cnt - 1] != v)
            coins[coin_cnt++] = v;
    }

    /* Process each coin value one by one. */
    for (size_t ci = 0; ci < coin_cnt; ++ci) {
        idx_t v = coins[ci];
        if (v == 0) continue;

        /* Make a copy of the current dp into newdp. */
        seq_t *newdp = (seq_t *)calloc((size_t)(n + 1), sizeof(seq_t));
        if (!newdp) break;
        memcpy(newdp, dp, (size_t)(n + 1) * sizeof(seq_t));

        /* For every residue modulo v process the indices belonging
           to that residue as a separate 1‑dimensional array. */
        for (idx_t r = 0; r < v && r <= n; ++r) {
            idx_t maxj = (n - r) / v;          /* number of indices in this residue minus 1 */
            size_t cnt = (size_t)maxj + 1;

            /* Allocate temporary arrays for values and prefix sums. */
            seq_t *arr     = (seq_t *)calloc(cnt, sizeof(seq_t));
            seq_t *prefix  = (seq_t *)calloc(cnt, sizeof(seq_t));
            if (!arr || !prefix) { free(newdp); free(arr); free(prefix); continue; }

            /* Fill arr with current dp values for this residue. */
            for (size_t j = 0; j < cnt; ++j) {
                idx_t idx = r + (idx_t)j * v;
                arr[j] = dp[idx];
            }

            /* Build prefix sums of arr. */
            prefix[0] = arr[0];
            for (size_t j = 1; j < cnt; ++j)
                prefix[j] = prefix[j - 1] + arr[j];

            /* Update newdp using the recurrence. */
            for (size_t j = 0; j < cnt; ++j) {
                idx_t idx = r + (idx_t)j * v;
                seq_t val = arr[j];
                if (j >= 10) val += prefix[j - 10];
                newdp[idx] = val;
            }

            free(arr);
            free(prefix);
        }

        /* Prepare for next coin: newdp becomes the current dp. */
        free(dp);
        dp = newdp;
    }

    seq_t result = dp[(size_t)n];
    free(dp);
    free(coins);
    return result;
}
