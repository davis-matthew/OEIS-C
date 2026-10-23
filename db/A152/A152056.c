/*
 * A152056  a(n) = ((9+sqrt(3))^n + (9-sqrt(3))^n)/2.
 * https://oeis.org/A152056
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ((9+sqrt(3))^n + (9-sqrt(3))^n)/2
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 18, u128 n <= 37,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0)=1, a(1)=9, a(n)=18*a(n-1)-78*a(n-2) for n>1 */

#include <stdlib.h>

/* Static cache for memoisation */
static seq_t *cache = NULL;          /* array of computed terms */
static size_t cache_capacity = 0;    /* allocated number of elements */
static idx_t cache_len = 0;          /* number of terms currently stored */

/* Ensure the cache can hold indices up to n (inclusive) */
static void ensure_cache(idx_t n)
{
    if ((size_t)(n + 1) <= cache_capacity)
        return;

    size_t new_cap = cache_capacity ? cache_capacity : 2;
    while (new_cap <= (size_t)n)
        new_cap <<= 1;  /* double until large enough */

    seq_t *new_cache;
    if (cache == NULL) {
        new_cache = (seq_t *)malloc(new_cap * sizeof(seq_t));
    } else {
        new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    }
    if (!new_cache) {
        /* Allocation failed – abort – this is fatal for the sequence */
        abort();
    }

    cache = new_cache;
    cache_capacity = new_cap;
}

/* Compute A152056(n) with memoised recurrence */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;
    if (n == 1)
        return (seq_t)9;

    ensure_cache(n);

    /* Initialise base cases if cache is empty */
    if (cache_len == 0) {
        cache[0] = (seq_t)1;  /* a(0) */
        cache[1] = (seq_t)9;  /* a(1) */
        cache_len = 2;
    }

    /* Compute missing terms up to n */
    for (idx_t i = cache_len; i <= n; ++i) {
        cache[i] = ((seq_t)18 * cache[i-1]) - ((seq_t)78 * cache[i-2]);
    }

    if ((idx_t)(cache_len) < n + 1)
        cache_len = n + 1;

    return cache[n];
}
