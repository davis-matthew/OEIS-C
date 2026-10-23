/*
 * A118030  a(1)=1. a(n) = a(n-1) + (largest integer which is <= n and occurs
 *          among the earlier terms of the sequence).
 * https://oeis.org/A118030
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = a(n-1) + (largest integer <= n that occurs among the
 *            earlier terms) Initial values: a(1)=1 Recurrence uses only
 *            integer arithmetic
 * Range:     u8 n <= 23, u16 n <= 369, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* static cache for computed terms */
    static seq_t *cache = NULL;
    static idx_t cache_count = 0;
    static size_t cache_cap = 0;

    if (n == 0) return 0; /* outside offset, undefined but safe */

    /* Ensure base term a(1)=1 exists */
    if (cache_count == 0) {
        cache_cap = 1;
        cache = (seq_t *)realloc(NULL, cache_cap * sizeof(seq_t));
        cache[0] = (seq_t)1;
        cache_count = 1;
    }

    /* Extend cache up to requested n */
    while (cache_count < n) {
        /* Ensure capacity */
        if (cache_count >= cache_cap) {
            size_t new_cap = cache_cap * 2;
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp) abort(); /* allocation failed */
            cache = tmp;
            cache_cap = new_cap;
        }

        idx_t next_n = cache_count + 1;               /* computing a(next_n) */
        seq_t f;

        /* Binary search for largest term <= next_n among previous terms */
        idx_t lo = 0, hi = cache_count;               /* cache[0..cache_count-1] */
        while (lo < hi) {
            idx_t mid = lo + (hi - lo) / 2;
            if (cache[mid] <= (seq_t)next_n) lo = mid + 1;
            else hi = mid;
        }
        /* lo is number of terms <= next_n; largest such term is cache[lo-1] */
        f = cache[lo - 1];

        /* Compute a(next_n) */
        cache[cache_count] = cache[cache_count - 1] + f;
        cache_count++;
    }

    return cache[n - 1];
}
