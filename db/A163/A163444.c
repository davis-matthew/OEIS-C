/*
 * A163444  a(n) = 12*a(n-1) - 34*a(n-2) for n > 1; a(0) = 1, a(1) = 8.
 * https://oeis.org/A163444
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 12*a(n-1) - 34*a(n-2) for n > 1; a(0) = 1, a(1) = 8
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 22, u128 n <= 44,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Cached values of the sequence. */
static seq_t  *seq_cache = NULL;   /* array of computed terms   */
static idx_t   cache_len  = 0;     /* number of terms stored   */
static size_t  cache_cap  = 0;     /* current allocated capacity */

/* Ensure the cache can hold terms up to index n (inclusive). */
static void ensure_cache(idx_t n)
{
    if (seq_cache == NULL) {
        /* Initial allocation – at least two terms to seed the recurrence. */
        cache_cap = (n + 1 > (idx_t)2) ? (size_t)(n + 1) : 2;
        seq_cache = (seq_t*)malloc(cache_cap * sizeof(seq_t));
        seq_cache[0] = (seq_t)1;
        seq_cache[1] = (seq_t)8;
        cache_len  = 2;
        return;
    }
    if ((size_t)n + 1 > cache_cap) {
        /* Grow capacity, doubling until it accommodates n. */
        while ((size_t)n + 1 > cache_cap) {
            cache_cap = (cache_cap == 0) ? 2 : cache_cap * 2;
        }
        seq_cache = (seq_t*)realloc(seq_cache, cache_cap * sizeof(seq_t));
    }
}

/* Compute a(n).  Recurrence is evaluated lazily and cached. */
seq_t a(idx_t n)
{
    ensure_cache(n);

    if (n < cache_len) {
        /* Already computed. */
        return seq_cache[n];
    }

    /* Compute missing terms up to n. */
    for (idx_t i = cache_len; i <= n; ++i) {
        seq_t term = 12 * seq_cache[i - 1] - 34 * seq_cache[i - 2];
        seq_cache[i] = term;
    }
    cache_len = n + 1;

    return seq_cache[n];
}
