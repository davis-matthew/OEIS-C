/*
 * A164593  a(n) = ((5 + sqrt(18))*(2 + sqrt(8))^n + (5 - sqrt(18))*(2 -
 *          sqrt(8))^n)/2.
 * https://oeis.org/A164593
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4*a(n-1) + 4*a(n-2), a(0) = 5, a(1) = 22
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 13, u64 n <= 27, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;   /* cached values */
static size_t cache_size = 0; /* number of initialized entries */
static size_t cache_cap  = 0; /* allocated buffer capacity */

/* Ensure that the cache can hold at least 'need' entries. */
static void ensure_capacity(size_t need)
{
    if (need <= cache_cap)
        return;

    size_t new_cap = cache_cap ? cache_cap << 1 : 2;
    while (new_cap < need)
        new_cap <<= 1;

    /* `realloc` returns a void*, cast it to seq_t* (safe in C++ as well). */
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    cache = tmp;          /* assume success; the caller will observe == NULL if it fails */
    cache_cap = new_cap;
}

/* Return the nth term of A164593, 0‑based.  The function is safe for any
   order of calls because the cache is updated lazily up to the required
   index. */
seq_t a(idx_t n)
{
    /* Handle the base cases without touching the cache. */
    if (n == 0)
        return (seq_t)5;
    if (n == 1)
        return (seq_t)22;

    /* Initialise the cache on first call. */
    if (cache == NULL) {
        ensure_capacity(2);
        cache[0] = (seq_t)5;
        cache[1] = (seq_t)22;
        cache_size = 2;
    }

    /* Extend the cache up to n, if necessary. */
    if ((size_t)n >= cache_size) {
        ensure_capacity((size_t)n + 1);
        for (size_t i = cache_size; i <= (size_t)n; ++i) {
            /* recurrence: a(i) = 4*a(i-1) + 4*a(i-2) */
            seq_t val = 4 * cache[i - 1] + 4 * cache[i - 2];
            cache[i] = val;
        }
        cache_size = (size_t)n + 1;
    }

    return cache[(size_t)n];
}
