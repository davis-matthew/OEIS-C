/*
 * A125953  a(0)=1. a(n) = sum{k=0 to n-1} (a(k)(mod n)).
 * https://oeis.org/A125953
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(0) = 1; for n >= 1, a(n) = Σ_{k=0}^{n-1} (a(k) mod n)
 * Range:     u8 n <= 25, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Static cache of computed terms */
static seq_t *cache   = NULL; /* array of a(n) values */
static size_t cache_cap = 0;   /* allocated capacity (in elements) */
static size_t cache_size = 0;  /* number of elements actually stored */

/* Ensure the cache can hold at least n+1 elements (indices 0 … n). */
static void ensure_cache_cap(idx_t n)
{
    if ((size_t)n < cache_cap)
        return;

    size_t new_cap = cache_cap ? cache_cap << 1 : 1;
    /* Grow until capacity > n */
    while ((size_t)n >= new_cap)
        new_cap <<= 1;

    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (tmp == NULL)
        abort();  /* allocation failed */
    cache = tmp;
    cache_cap = new_cap;
}

/* Return a(n).  Memoises the sequence so that calls in any order are
 * efficient. */
seq_t a(idx_t n)
{
    if (n < cache_size)
        return cache[n];

    /* Initialise for the first call. */
    if (cache_size == 0) {
        ensure_cache_cap(0);
        cache[0] = (seq_t)1;
        cache_size = 1;
    }

    /* Ensure capacity for all indices up to n. */
    if ((size_t)n >= cache_cap)
        ensure_cache_cap(n);

    /* Compute terms from cache_size up to n inclusive. */
    for (idx_t i = (idx_t)cache_size; i <= n; ++i) {
        seq_t sum = 0;
        seq_t lim = (seq_t)i;           /* modulus is i */
        for (idx_t k = 0; k < i; ++k)
            sum += cache[k] % lim;
        cache[i] = sum;
    }

    cache_size = (size_t)n + 1; /* now have terms up to n */
    return cache[n];
}
