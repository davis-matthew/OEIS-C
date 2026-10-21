/*
 * A055432  Number of points in Z^n of norm <= 8.
 * https://oeis.org/A055432
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = number of integer vectors (x1,…,xn) in Z^n with norm ≤ 8
 *            (i.e. sum of squares xi^2 ≤ 64). Computed via dynamic
 *            programming of the generating function 1 + 2(x + x^4 + x^9 + … +
 *            x^64). The function memoises results so that any call order
 *            works and stays well under 2 s for n≈10000
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 10, u64 n <= 24, u128 n <= 64,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* memoisation store */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;
    static idx_t current_n = 0;
    static seq_t dp[65];     /* distribution for the current n */
    static bool initialized = false;

    if (!initialized) {
        /* initialise for n = 0 */
        for (idx_t s = 0; s < 65; ++s) dp[s] = 0;
        dp[0] = 1;

        cache_len = 1;
        cache = (seq_t *)malloc(sizeof(seq_t));
        if (!cache) return 0;      /* allocation failure – unlikely for this use */
        cache[0] = 1;

        initialized = true;
    }

    if (n >= cache_len) {
        /* expand cache and compute up to n */
        const int squares[8] = {1,4,9,16,25,36,49,64};
        cache = (seq_t *)realloc(cache, (size_t)(n + 1) * sizeof(seq_t));
        if (!cache) return 0;

        seq_t new_dp[65];

        for (idx_t step = current_n; step < n; ++step) {
            /* reset new state */
            for (idx_t s = 0; s < 65; ++s) new_dp[s] = 0;

            /* convolve current dp with g(x) = 1 + 2 ∑_{sq>0} x^sq */
            for (idx_t s = 0; s < 65; ++s) {
                seq_t val = dp[s];
                if (val == 0) continue;

                /* zero square */
                new_dp[s] += val;

                /* non‑zero squares – add twice to avoid overflow */
                for (int j = 0; j < 8; ++j) {
                    idx_t sp = s + squares[j];
                    if (sp < 65) {
                        new_dp[sp] += val;
                        new_dp[sp] += val;
                    }
                }
            }

            /* copy back to dp */
            for (idx_t s = 0; s < 65; ++s) dp[s] = new_dp[s];

            /* sum all dp[s] to obtain a(step+1) */
            seq_t sum = 0;
            for (idx_t s = 0; s < 65; ++s) sum += dp[s];
            cache[step + 1] = sum;
        }

        current_n = n;
        cache_len = n + 1;
    }

    return cache[n];
}
