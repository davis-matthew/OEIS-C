/*
 * A047717  Numbers that are the sum of 6 but no fewer nonzero fourth powers.
 * https://oeis.org/A047717
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the n-th positive integer that can be expressed as the
 *            sum of exactly 6 non‑zero fourth powers and cannot be expressed
 *            as the sum of fewer than 6 such powers
 * Range:     u8 n <= 18, u16 n <= 7762, u32 n <= 9892, u64 n <= 9892,
 *            u128 n <= 9892, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The sequence is computed by a normal unbounded knapsack DP.
   dp[x] = minimal number of fourth‑power summands needed to obtain x.
   Numbers with dp[x]==6 are stored in cache.  The table is built
   once and reused on subsequent calls. */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

/* Choose a bound large enough to contain at least 10 000 terms.
   5 000 000 is sufficient; the DP table is about 5 MB. */
#define BOUND 5000000
#define DESIRED_TERMS 10000

/* Use unsigned char for dp (max value needed is 7). */
static unsigned char dp[BOUND + 1];

/* Pre‑computed list of fourth powers ≤ BOUND. */
static idx_t fourth_pows[50];
static size_t fp_cnt = 0;

/* Cache of first DESIRED_TERMS values of the sequence. */
static seq_t cache[DESIRED_TERMS];
static size_t cache_sz = 0;

/* Flag for one‑time initialization. */
static int initialized = 0;

seq_t a(idx_t n) {
    if (!initialized) {
        /* ---------- Initialize dp table ---------- */
        memset(dp, 7, sizeof(dp));   /* 7 = INF (greater than 6) */
        dp[0] = 0;

        /* ----- Compute list of fourth powers ----- */
        for (idx_t k = 1; ; ++k) {
            uint64_t v = 1ULL * k * k * k * k;
            if (v > BOUND)
                break;
            fourth_pows[fp_cnt++] = (idx_t)v;
        }

        /* ----- DP: compute minimal term counts ---------- */
        for (size_t i = 0; i < fp_cnt; ++i) {
            idx_t v = fourth_pows[i];
            for (idx_t x = v; x <= BOUND; ++x) {
                unsigned char cand = dp[x - v] + 1;
                if (cand < dp[x])
                    dp[x] = cand;
            }
        }

        /* ----- Extract first DESIRED_TERMS terms ---------- */
        for (idx_t x = 1; x <= BOUND && cache_sz < DESIRED_TERMS; ++x) {
            if (dp[x] == 6) {
                cache[cache_sz++] = (seq_t)x;
            }
        }
        initialized = 1;
    }

    /* The caller guarantees 1 ≤ n ≤ DESIRED_TERMS. */
    return cache[n - 1];
}
