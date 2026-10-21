/*
 * A240013  Number of partitions of n, where the difference between the number
 *          of odd parts and the number of even parts is 4.
 * https://oeis.org/A240013
 *
 * Offset:    4    (a(4) is the first term)
 * Keywords:  nonn
 * Formula:   Number of partitions of n where the difference between the
 *            number of odd parts and the number of even parts equals 4. The
 *            algorithm evaluates the generating function Σ_{k≥0} x^{3k+4} / (
 *            ∏_{j=1..k} (1-x^{2j}) * ∏_{j=1..k+4} (1-x^{2j}) ) by iteratively
 *            building the even‑part product and accumulati...
 * Range:     u8 n <= 27, u16 n <= 61, u32 n <= 160, u64 n <= 481,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>

/* External integer types supplied by the build. */
extern seq_t a(idx_t n);

/* The function is defined below. */
seq_t a(idx_t n)
{
    /* Return 0 for indices below the offset. */
    if (n < 4) return 0;

    /* Static Cache: array of precomputed terms up to some capacity. */
    static seq_t *cache = NULL;      /* array [0 .. capacity-1] */
    static idx_t capacity = 0;       /* number of stored entries */

    /* If requested index exceeds cache, extend and recompute up to that size. */
    if (n >= capacity) {
        idx_t new_cap = n + 1;  /* we need indices 0 .. n inclusive */

        /* Allocate new cache and copy the old part. */
        seq_t *new_cache = (seq_t*)malloc(new_cap * sizeof(seq_t));
        if (!new_cache) return 0;             /* allocation failed */

        /* Initialize new area to zero. */
        if (capacity < new_cap)
            memset(new_cache + capacity, 0, (new_cap - capacity) * sizeof(seq_t));

        /* Copy existing values if present. */
        if (cache != NULL) {
            memcpy(new_cache, cache, capacity * sizeof(seq_t));
            free(cache);
        }

        /* Temporary arrays for dynamic programming. */
        seq_t *dp   = (seq_t*)calloc(new_cap, sizeof(seq_t)); /* dp[i] : coeff of x^i in current product */
        seq_t *a_arr = (seq_t*)calloc(new_cap, sizeof(seq_t));/* partial results a_arr[i] */

        if (dp && a_arr) {
            /* Base product for k = 0:
               ∏_{j=1}^{4} (1 - x^{2j})^{-1}  */
            dp[0] = 1;
            for (idx_t j = 1; j <= 4; ++j) {
                idx_t v = 2 * j;
                if (v >= new_cap) continue;
                for (idx_t i = v; i < new_cap; ++i)
                    dp[i] += dp[i - v];
            }

            /* Maximum k that contributes for this size. */
            idx_t Kmax = (new_cap > 4) ? ((new_cap - 1) - 4) / 3 : 0;

            /* Main loop over k. */
            for (idx_t k = 0; k <= Kmax; ++k) {
                idx_t shift = 3 * k + 4;

                /* Add dp contribution shifted by shift. */
                for (idx_t i = shift; i < new_cap; ++i)
                    a_arr[i] += dp[i - shift];

                /* No further dp updates after the last k. */
                if (k == Kmax) break;

                /* Update dp for next k:
                   multiply by (1 - x^{2(k+1)})^{-1} and (1 - x^{2(k+5)})^{-1}. */
                idx_t v1 = 2 * (k + 1);
                if (v1 < new_cap) {
                    for (idx_t i = v1; i < new_cap; ++i)
                        dp[i] += dp[i - v1];
                }

                idx_t v2 = 2 * (k + 5);
                if (v2 < new_cap) {
                    for (idx_t i = v2; i < new_cap; ++i)
                        dp[i] += dp[i - v2];
                }
            }

            /* Copy the computed a_arr into the cache. */
            for (idx_t i = 0; i < new_cap; ++i)
                new_cache[i] = a_arr[i];
        }

        free(dp);
        free(a_arr);

        /* Update global cache reference. */
        cache = new_cache;
        capacity = new_cap;
    }

    /* The requested term is now in the cache. */
    return cache[n];
}
