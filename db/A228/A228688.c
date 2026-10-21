/*
 * A228688  Number of 7Xn binary arrays with no two ones adjacent
 *          horizontally, diagonally or antidiagonally.
 * https://oeis.org/A228688
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 9*a(n-1) +184*a(n-2) -1201*a(n-3) -7155*a(n-4)
 *            +46048*a(n-5) + * 70745*a(n-6) -591673*a(n-7) -154744*a(n-8)
 *            +3207657*a(n-9) -715173*a(n-10) - * 7580648*a(n-11)
 *            +3369552*a(n-12) +7114920*a(n-13) -3800952*a(n-14) - *
 *            1923264*a(n-15) +1083456*a(n-16) for n >= 17 * Initial values
 *            are computed...
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 15, u128 n <= 32,
 *            big any n
 * Helpers:   mul_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "../utils/mul_small.h"      /* seq_t mul_small(seq_t x, unsigned int m); */

static bool transfer_inited = false;
static bool allowed[128][128];

/* precompute transition matrix: transition from column s to column t is allowed if
 * no two 1s are adjacent horizontally, diagonally or antidiagonally. */
static void init_transfer_matrix(void)
{
    if (transfer_inited) return;
    const unsigned int MASK = 0x7F; /* keep only 7 bits */
    for (int s = 0; s < 128; ++s) {
        for (int t = 0; t < 128; ++t) {
            if ((s & t) != 0) {                    /* horizontal (same row) conflict   */
                allowed[s][t] = false;
                continue;
            }
            if ((s & (t >> 1)) != 0) {            /* diagonal down-right conflict    */
                allowed[s][t] = false;
                continue;
            }
            if ((s & ((t << 1) & MASK)) != 0) {    /* diagonal up-right conflict      */
                allowed[s][t] = false;
                continue;
            }
            allowed[s][t] = true;
        }
    }
    transfer_inited = true;
}

/* compute a(n) for any n >= 1 */
seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;

    /* initialise cache and first 16 terms on first call */
    if (cache == NULL) {
        cache_cap = 32;                       /* reserve at least 32 entries */
        cache = (seq_t *)calloc(cache_cap + 1, sizeof(seq_t));
        init_transfer_matrix();

        /* DP for the first 16 columns */
        seq_t dp[128], ndp[128];
        for (int m = 0; m < 128; ++m) dp[m] = 1;   /* width 1 */
        seq_t total = 0;
        for (int m = 0; m < 128; ++m) total += dp[m];
        cache[1] = total;                          /* a(1) = 128 */

        for (int col = 2; col <= 16; ++col) {
            /* reset ndp */
            for (int t = 0; t < 128; ++t) ndp[t] = 0;
            /* transition */
            for (int s = 0; s < 128; ++s) {
                seq_t v = dp[s];
                if (!v) continue;
                for (int t = 0; t < 128; ++t)
                    if (allowed[s][t]) ndp[t] += v;
            }
            /* copy ndp back to dp */
            for (int m = 0; m < 128; ++m) dp[m] = ndp[m];
            /* sum of all states gives a(col) */
            total = 0;
            for (int m = 0; m < 128; ++m) total += dp[m];
            cache[col] = total;
        }
    }

    /* expand cache if necessary */
    if ((size_t)n > cache_cap) {
        size_t newcap = cache_cap;
        while (newcap <= (size_t)n) newcap <<= 1;
        seq_t *newcache = (seq_t *)calloc(newcap + 1, sizeof(seq_t));
        memcpy(newcache, cache, (cache_cap + 1) * sizeof(seq_t));
        free(cache);
        cache = newcache;
        cache_cap = newcap;
    }

    /* compute terms beyond 16 by the recurrence */
    for (size_t i = 17; i <= (size_t)n; ++i) {
        if (cache[i] != 0) continue;   /* already computed */

        static const int coeffs[16] = { 9, 184, -1201, -7155, 46048, 70745,
                                        -591673, -154744, 3207657, -715173,
                                        -7580648, 3369552, 7114920, -3800952,
                                        -1923264, 1083456 };

        seq_t pos_sum = 0;
        seq_t neg_sum = 0;
        for (int k = 0; k < 16; ++k) {
            seq_t val = cache[i - 1 - k];
            if (coeffs[k] >= 0)
                pos_sum += mul_small(val, (unsigned int)coeffs[k]);
            else
                neg_sum += mul_small(val, (unsigned int)(-coeffs[k]));
        }
        seq_t result = pos_sum - neg_sum;
        cache[i] = result;
    }

    return cache[n];
}
