/*
 * A214899  a(n) = a(n-1) + a(n-2) + a(n-3) with a(0)=2, a(1)=1, a(2)=2.
 * https://oeis.org/A214899
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=2, a(1)=1, a(2)=2, a(n)=a(n-1)+a(n-2)+a(n-3) for n≥3
 *            Sequence A214899 – tribonacci‑type recurrence with offset 0
 * Range:     u8 n <= 9, u16 n <= 18, u32 n <= 36, u64 n <= 73, u128 n <= 146,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;           /* dynamically allocated array of terms */
static idx_t cache_len = 0;          /* allocated capacity (in elements)       */
static idx_t max_cached = (idx_t)-1; /* highest n for which cache[n] is valid   */

seq_t a(idx_t n)
{
    /* Lazy allocation and initialization of the first three terms. */
    if (cache == NULL) {
        cache_len = 3;
        cache = (seq_t *)malloc(cache_len * sizeof(seq_t));
        if (cache == NULL) return (seq_t)0;        /* allocation failed */
        cache[0] = (seq_t)2;
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)2;
        max_cached = 2;
    }

    /* Return cached value if already computed. */
    if (n < cache_len && n <= max_cached) {
        return cache[n];
    }

    /* Grow the cache to accommodate n. */
    if (n >= cache_len) {
        idx_t new_len = cache_len;
        while (new_len <= n) new_len <<= 1;       /* double until large enough */

        seq_t *tmp = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
        if (tmp == NULL) {
            free(cache);
            cache = NULL;
            cache_len = 0;
            return (seq_t)0;                     /* allocation failed */
        }
        cache = tmp;
        cache_len = new_len;
    }

    /* Compute and store all missing terms up to n. */
    for (idx_t i = max_cached + 1; i <= n; ++i) {
        cache[i] = cache[i - 1] + cache[i - 2] + cache[i - 3];
    }
    max_cached = n;

    return cache[n];
}
