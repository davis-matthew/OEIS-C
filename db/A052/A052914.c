/*
 * A052914  Expansion of (1-x)/(1 - x - x^3 - 2*x^4 + 2*x^5).
 * https://oeis.org/A052914
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = a(n-1)+a(n-3)+2*a(n-4)-2*a(n-5), a(0)=1, a(1)=0, a(2)=0,
 *            a(3)=1, a(4)=3
 * Range:     u8 n <= 15, u16 n <= 27, u32 n <= 51, u64 n <= 99, u128 n <= 196,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

// Implements OEIS sequence A052914

#include <stdlib.h>  // for realloc

static seq_t *cache = NULL;        /* dynamic memoization array */
static size_t cache_cap = 0;       /* allocated size of cache */
static idx_t  max_computed = (idx_t)-1; /* highest index whose value is stored */

/* Ensure the cache can hold indices up to 'idx'.
   If the cache is empty, allocate it and store the first five terms. */
static void assure_cache(idx_t idx)
{
    if (cache == NULL) {
        /* initial allocation; at least 12 entries for growth */
        cache_cap = 12;
        cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
        if (!cache) abort(); /* unrecoverable allocation failure */

        /* initialise the first 5 terms (indices 0..4) */
        cache[0] = (seq_t)1;  /* a(0) */
        cache[1] = (seq_t)0;  /* a(1) */
        cache[2] = (seq_t)0;  /* a(2) */
        cache[3] = (seq_t)1;  /* a(3) */
        cache[4] = (seq_t)3;  /* a(4) */
        max_computed = 4;
    }

    /* grow the cache if needed */
    size_t needed = (size_t)idx + 1;
    if (needed > cache_cap) {
        size_t new_cap = cache_cap * 2;
        if (new_cap < needed) new_cap = needed;
        cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!cache) abort();
        cache_cap = new_cap;
    }
}

seq_t a(idx_t n)
{
    /* handle the base values immediately */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)0;
    if (n == 2) return (seq_t)0;
    if (n == 3) return (seq_t)1;
    if (n == 4) return (seq_t)3;

    assure_cache(n);

    /* extend the cached values up to n, if necessary */
    if (n > max_computed) {
        for (size_t i = max_computed + 1; i <= (size_t)n; ++i) {
            seq_t t = cache[i-1] + cache[i-3] + (seq_t)2 * cache[i-4];
            t -= (seq_t)2 * cache[i-5];
            cache[i] = t;
        }
        max_computed = n;
    }

    return cache[n];
}
