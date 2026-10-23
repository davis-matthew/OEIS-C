/*
 * A046090  Consider all Pythagorean triples (X,X+1,Z) ordered by increasing
 *          Z; sequence gives X+1 values.
 * https://oeis.org/A046090
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, nice, changed
 * Formula:   a(n) = 6*a(n-1) - a(n-2) - 2, a(0)=1, a(1)=4
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 25, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;      /* cached terms, 0‑based indices */
static size_t cache_len = 0;     /* number of terms currently stored (≤ cache_cap) */
static size_t cache_cap = 0;     /* allocated capacity of cache */

/* Ensure that the cache can hold at least 'need' terms (0‑based). */
static void extend_cache(size_t need)
{
    if (need <= cache_cap) return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 8;
    while (new_cap < need) new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) abort();
    cache = tmp;
    cache_cap = new_cap;
}

/* Return the n-th term of sequence A046090. */
seq_t a(idx_t n)
{
    /* Ensure cache is large enough for this index. */
    size_t need = (size_t)n + 1;
    extend_cache(need);

    /* If already cached, return it. */
    if (n < cache_len) return cache[n];

    /* Initialize first two terms if necessary. */
    if (cache_len == 0) {
        cache[0] = (seq_t)1;
        cache_len = 1;
    }
    if (cache_len == 1) {
        cache[1] = (seq_t)4;
        cache_len = 2;
    }

    /* Compute missing terms up to n. */
    for (size_t i = cache_len; i <= (size_t)n; ++i) {
        cache[i] = (cache[i-1] * (seq_t)6) - cache[i-2] - (seq_t)2;
    }
    cache_len = need;
    return cache[n];
}
