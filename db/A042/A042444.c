/*
 * A042444  Numerators of continued fraction convergents to sqrt(750).
 * https://oeis.org/A042444
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 10, u32 n <= 20, u64 n <= 45, u128 n <= 90,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Continued-fraction numerators of sqrt(750).
   Recurrence:  p_-2 = 0, p_-1 = 1;
   for k ≥ 0   p_k = a_k·p_{k-1} + p_{k-2},
   where a_0 = ⌊√750⌋ = 27 and a_k for k ≥ 1 are the periodic partial
   quotients of the continued fraction of √750. */
/*=====================================================================*/

#include <stdlib.h>

/* Main function required by the build system */
seq_t a(idx_t n)
{
    /* One‑time data: period of the continued fraction and cache of
       numerators.  All are held in static storage so the function
       works correctly for repeated or out‑of‑order calls. */
    static int initialized = 0;
    static idx_t a0 = 0;                     /* ⌊√750⌋ */
    static idx_t *period = NULL;             /* periodic a_k for k>=1 */
    static size_t period_len = 0;
    static size_t period_cap = 0;

    static seq_t *cache = NULL;              /* cache of numerators p_k */
    static size_t cache_len = 0;             /* number of elements stored */
    static size_t cache_cap = 0;             /* allocated capacity */

    /* ---------- one‑time initialisation ---------- */
    if (!initialized) {
        /* compute a0 = floor(sqrt(750)) */
        a0 = 0;
        while ((a0 + 1) * (a0 + 1) <= (idx_t)750)
            a0++;

        /* periodic part of the continued fraction of √750 */
        idx_t m = 0;
        idx_t d = 1;
        idx_t a_val = a0;

        period_cap = 32;
        period = (idx_t *)malloc(period_cap * sizeof(idx_t));
        period_len = 0;

        for (;;) {
            m = d * a_val - m;
            d = (750 - m * m) / d;
            a_val = (a0 + m) / d;

            /* grow period if required */
            if (period_len >= period_cap) {
                period_cap <<= 1;
                period = (idx_t *)realloc(period, period_cap * sizeof(idx_t));
            }
            period[period_len++] = a_val;

            if (a_val == 2 * a0)   /* period terminates |
                                     * when a_k = 2*a0                */
                break;
        }

        /* initialise cache for numerators */
        cache_cap = 16;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        cache_len = 0;

        initialized = 1;
    }

    /* ---------- extend the cache up to n ---------- */
    if ((idx_t)n >= cache_len) {
        size_t need = (size_t)n + 1;
        if (need > cache_cap) {
            size_t new_cap = cache_cap;
            while (new_cap < need) new_cap <<= 1;
            cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            cache_cap = new_cap;
        }

        for (size_t k = cache_len; k <= (size_t)n; ++k) {
            if (k == 0) {
                /* p_0 = a0 */
                cache[0] = (seq_t)a0;
            } else {
                /* partial quotient a_k (k≥1) comes from the period   */
                size_t idx = (size_t)((k - 1) % period_len);
                idx_t a_k = period[idx];          /* fits easily in idx_t */

                seq_t prev1 = cache[k - 1];
                seq_t prev2 = (k >= 2) ? cache[k - 2] : (seq_t)1;
                cache[k] = (seq_t)a_k * prev1 + prev2;
            }
        }
        cache_len = (size_t)n + 1;
    }

    return cache[(size_t)n];
}
