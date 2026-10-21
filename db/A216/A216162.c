/*
 * A216162  Sequences A006452 and A216134 interlaced.
 * https://oeis.org/A216162
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-2) + 6*a(n-4) - 6*a(n-6) - a(n-8) + a(n-10) with
 *            initial values a(0)=1, a(1)=0, a(2)=1, a(3)=1, a(4)=2, a(5)=4,
 *            a(6)=4, a(7)=9, a(8)=11, a(9)=26
 * Range:     u8 n <= 14, u16 n <= 27, u32 n <= 52, u64 n <= 102,
 *            u128 n <= 203, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    /* static cache for memoisation */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;   /* number of values stored (>= cache_len) */

    /* initial values up to n = 9 */
    const static seq_t init[10] = {
        (seq_t)1, (seq_t)0, (seq_t)1, (seq_t)1, (seq_t)2,
        (seq_t)4, (seq_t)4, (seq_t)9, (seq_t)11, (seq_t)26
    };

    /* ensure cache can hold up to n */
    if (cache_len <= (size_t)n) {
        size_t needed = (size_t)n + 1;
        if (!cache) {
            cache = (seq_t*)malloc(needed * sizeof(seq_t));
            if (!cache) return (seq_t)0; /* allocation failure */
            cache_len = 0;
        } else if (cache_len < needed) {
            seq_t *new_cache = (seq_t*)realloc(cache, needed * sizeof(seq_t));
            if (!new_cache) return (seq_t)0; /* allocation failure */
            cache = new_cache;
        }
        /* initialise missing entries with the constant table */
        for (size_t i = cache_len; i < 10 && i < needed; ++i)
            cache[i] = init[i];
        /* for indices beyond 9, they will be computed later */
        cache_len = needed;
    }

    /* compute new terms using the 10‑term linear recurrence */
    if (cache_len > 10) {
        for (size_t i = 10; i < cache_len; ++i) {
            seq_t diff1 = cache[i-2] - cache[i-8];
            seq_t diff2 = cache[i-4] - cache[i-6];
            cache[i] = diff1 + diff2 * (seq_t)6 + cache[i-10];
        }
    }

    return cache[(size_t)n];
}
