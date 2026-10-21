/*
 * A053801  Number of basis partitions of n+36 with Durfee square size 6.
 * https://oeis.org/A053801
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 10, u16 n <= 38, u32 n <= 356, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   Recurrence for A053801:
   a(0) = 1, a(1) = 2, a(2) = 4, a(3) = 8, a(4) = 14, a(5) = 24,
   a(6) = 40, a(7) = 62, a(8) = 94, a(9) = 140, a(10) = 202,
   a(11) = 286, a(12) = 398, a(13) = 542, a(14) = 728
   For n > 14:
   a(n) = 3*a(n-1) - 3*a(n-2) + 3*a(n-3) - 6*a(n-4) + 7*a(n-5)
          - 6*a(n-6) + 6*a(n-7) - 6*a(n-8) + 7*a(n-9) - 6*a(n-10)
          + 3*a(n-11) - 3*a(n-12) + 3*a(n-13) - a(n-14)
*/

#include <stdlib.h>

seq_t a(idx_t n) {
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;
    static idx_t cache_cap = 0;

    /* Ensure capacity for at least the requested index and the 15 initial terms */
    idx_t needed = (n + 1 > 15) ? n + 1 : 15;
    if (needed > cache_cap) {
        seq_t *new_cache = (seq_t *)malloc(sizeof(seq_t) * needed);
        if (!new_cache) return 0;          /* OOM – not expected in tests */
        for (idx_t i = 0; i < cache_size; ++i) {
            new_cache[i] = cache[i];
        }
        free(cache);
        cache = new_cache;
        cache_cap = needed;
    }

    /* Initialize the first 15 terms in the cache if not already done */
    if (cache_size < 15) {
        static const seq_t init[15] = {
            1ULL, 2ULL, 4ULL, 8ULL, 14ULL, 24ULL,
            40ULL, 62ULL, 94ULL, 140ULL, 202ULL,
            286ULL, 398ULL, 542ULL, 728ULL
        };
        for (idx_t i = cache_size; i < 15; ++i) {
            cache[i] = init[i];
        }
        cache_size = 15;
    }

    /* If the value is already computed, return it */
    if (n < cache_size) return cache[n];

    /* Compute missing terms up to n */
    for (idx_t i = cache_size; i <= n; ++i) {
        /* Positive part of the recurrence */
        seq_t pos = 0;
        pos += (seq_t)3 * cache[i-1];
        pos += (seq_t)3 * cache[i-3];
        pos += (seq_t)7 * cache[i-5];
        pos += (seq_t)6 * cache[i-7];
        pos += (seq_t)7 * cache[i-9];
        pos += (seq_t)3 * cache[i-11];
        pos += (seq_t)3 * cache[i-13];

        /* Negative part of the recurrence */
        seq_t neg = 0;
        neg += (seq_t)3 * cache[i-2];
        neg += (seq_t)6 * cache[i-4];
        neg += (seq_t)6 * cache[i-6];
        neg += (seq_t)6 * cache[i-8];
        neg += (seq_t)6 * cache[i-10];
        neg += (seq_t)3 * cache[i-12];
        neg += cache[i-14];

        cache[i] = pos - neg;
    }

    cache_size = n + 1;
    return cache[n];
}
