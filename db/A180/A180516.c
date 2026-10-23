/*
 * A180516  Numbers of the form i*4^j-1 (i=1..3, j >= 0).
 * https://oeis.org/A180516
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 13, u16 n <= 25, u32 n <= 49, u64 n <= 97, u128 n <= 193,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Sequence A180516: Numbers of the form i*4^j-1 (i=1..3, j≥0)
 * Recurrence: a(n) = a(n-1) + 4*a(n-3) - 4*a(n-4)
 * Initial values (offset 1): a(1)=0, a(2)=1, a(3)=2, a(4)=3
 * 
 * The function uses a static dynamic array to memoise previously
 * computed terms, so calls may arrive in arbitrary order.
 */

#include <stdlib.h>

/* Static cache for memoisation (indices 1..cache_cap-1, index 0 unused) */
static seq_t *cache = NULL;   /* pointer to cached values */
static size_t cache_cap = 0;  /* allocated size (≥ n+1) */

/* Ensure that cache contains values up to index n (1‑based) */
static void ensure_cache(idx_t n)
{
    /* Initial allocation if needed */
    if (cache == NULL) {
        /* Allocate enough for indices 1..max(4,n) + dummy 0 */
        cache_cap = (size_t)(n > 4 ? n + 1 : 5);
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) abort();

        /* Dummy index 0 */
        cache[0] = (seq_t)0;

        /* Initial terms */
        cache[1] = (seq_t)0;
        cache[2] = (seq_t)1;
        cache[3] = (seq_t)2;
        cache[4] = (seq_t)3;

        /* Compute any remaining values up to cache_cap-1 */
        for (size_t i = 5; i < cache_cap; ++i) {
            cache[i] = cache[i-1] + (cache[i-3] << 2) - (cache[i-4] << 2);
        }
        return;
    }

    /* Extend cache if necessary */
    size_t needed_cap = (size_t)n + 1;
    if (needed_cap <= cache_cap) return;   /* already computed */

    seq_t *tmp = (seq_t *)realloc(cache, needed_cap * sizeof(seq_t));
    if (!tmp) abort();
    cache = tmp;

    /* Compute new values from old cap up to needed_cap-1 */
    for (size_t i = cache_cap; i < needed_cap; ++i) {
        cache[i] = cache[i-1] + (cache[i-3] << 2) - (cache[i-4] << 2);
    }
    cache_cap = needed_cap;
}

/* Public accessor for the sequence */
seq_t a(idx_t n)
{
    if (n < 1) abort();       /* sequence is defined for n ≥ 1 */
    ensure_cache(n);
    return cache[(size_t)n];
}
