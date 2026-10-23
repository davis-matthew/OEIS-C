/*
 * A031588  Numbers k such that the continued fraction for sqrt(k) has even
 *          period and if the last term of the periodic part is deleted the
 *          central term is 90.
 * https://oeis.org/A031588
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 26, u32 n <= 40, u64 n <= 40, u128 n <= 40,
 *            big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A031588: numbers k such that the continued‑fraction expansion of √k
   has an even period and, after removing the last term, the central
   term of the remaining period equals 90.  The function returns the
   n‑th such number, with a(1) being the first term listed in the OEIS
   entry (8102).  Implementation uses a simple search with period
   detection.  */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/floor_sqrt.h"

seq_t a(idx_t n) {
    /* Memoisation cache */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;
    static idx_t cache_cap  = 0;

    /* Array for storing the period of √k */
    static seq_t *period = NULL;
    static size_t period_cap = 0;

    /* Next candidate k to test (skipping 0 and 1) */
    static idx_t next_k = 2;

    /* Generate terms until we have at least n */
    while (cache_size < n) {
        seq_t k = (seq_t)next_k;

        /* Skip perfect squares */
        seq_t a0 = floor_sqrt(k);
        if (a0 * a0 == k) {
            next_k = (idx_t)(k + 1);
            continue;
        }

        /* Build continued‑fraction period of √k */
        seq_t m = 0;
        seq_t d = 1;
        seq_t a_cf = a0;
        seq_t two_a0 = a0 + a0;

        /* Ensure capacity for the period array */
        if (period_cap < 64) {
            free(period);
            period_cap = 64;
            period = (seq_t*)malloc(period_cap * sizeof(seq_t));
            if (!period) exit(1);
        }

        size_t period_len = 0;
        while (true) {
            m = d * a_cf - m;
            d = (k - m * m) / d;
            a_cf = (a0 + m) / d;

            /* Grow period array if needed */
            if (period_len == period_cap) {
                size_t new_cap = period_cap * 2;
                seq_t *tmp = (seq_t*)realloc(period, new_cap * sizeof(seq_t));
                if (!tmp) exit(1);
                period = tmp;
                period_cap = new_cap;
            }

            period[period_len++] = a_cf;

            if (a_cf == two_a0) break;
        }

        /* Check the conditions: even period and central term 90 */
        if ((period_len & 1) == 0) {
            size_t mid = (period_len - 1) / 2;  /* index after deleting last term */
            if (period[mid] == (seq_t)90) {
                /* Store k in the cache */
                if (cache_size == cache_cap) {
                    size_t new_cap = cache_cap ? cache_cap * 2 : 8;
                    seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
                    if (!tmp) exit(1);
                    cache = tmp;
                    cache_cap = new_cap;
                }
                cache[cache_size++] = k;
            }
        }

        next_k = (idx_t)(k + 1);
    }

    return cache[n - 1];
}
