/*
 * A091544  First column sequence of array A091746 ((6,2)-Stirling2).
 * https://oeis.org/A091544
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) * 2 * (2*(n-1)+1) * (4*(n-1)+1), a(1)=1
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 5, u64 n <= 8, u128 n <= 14,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;
static idx_t cache_len = 0;   /* number of terms currently stored (a(1)..a(cache_len)) */
static idx_t cache_cap = 0;   /* allocated capacity */

/* Ensure that the cache holds terms up to index N (1‑based). */
static void ensure_cache(idx_t N)
{
    if (N < 1) return;

    /* initial allocation */
    if (cache_len == 0) {
        cache_cap = 16;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) abort();            /* malloc failed */
        cache[0] = (seq_t)1;            /* a(1) */
        cache_len = 1;
    }

    /* grow if needed */
    if (N > cache_cap) {
        size_t new_cap = cache_cap;
        while (new_cap < (size_t)N) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();
        cache = tmp;
        cache_cap = (idx_t)new_cap;
    }

    /* compute missing terms */
    for (idx_t n = cache_len + 1; n <= N; ++n) {   /* n is 1‑based index to compute */
        seq_t prev = cache[n - 2];                 /* a(n-1) */
        seq_t factor = (seq_t)2 *
                       (seq_t)(2 * (n - 1) + 1) *
                       (seq_t)(4 * (n - 1) + 1);
        cache[n - 1] = prev * factor;              /* a(n) */
    }
    cache_len = N;
}

/* Return the n‑th term of the sequence (offset 1). */
seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;   /* out of range; sequence defined for n>=1 */
    ensure_cache(n);
    return cache[n - 1];
}
