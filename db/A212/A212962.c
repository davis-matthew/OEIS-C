/*
 * A212962  Expansion of x*(3+x-x^3)/((1-3*x-x^2)*(1-x)*(1+x)).
 * https://oeis.org/A212962
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 18, u64 n <= 37, u128 n <= 74,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A212962 – Expansion of x*(3+x-x^3)/((1-3*x-x^2)*(1-x)*(1+x))
 *  Recurrence:
 *      a(0)=0,  a(1)=3,  a(2)=10,  a(3)=36,  a(4)=118
 *      a(n) = 3*a(n-1) + 2*a(n-2) - 3*a(n-3) - a(n-4)   for n ≥ 5
 *
 *  Computing is memoised in a static growing array; the sequence can be
 *  accessed in arbitrary order.  All arithmetic uses the supported
 *  operators on seq_t, and the array is grown with stdlib calls.
 */

#include <stddef.h>
#include <stdlib.h>

static seq_t *cache     = NULL;   /* memoisation array */
static idx_t cache_len   = 0;     /* allocated slots */
static idx_t max_computed = (idx_t)-1;  /* highest index with a computed value */

/* Ensure the cache can hold indices up to n, allocating or growing as needed. */
static void
ensure_cache(idx_t n)
{
    if (cache_len == 0) {
        /* allocate enough space for the first few terms (at least 5) */
        size_t new_len = (size_t)((n >= 4) ? n + 1 : 5);
        cache = (seq_t *)malloc(new_len * sizeof(seq_t));
        if (!cache) abort();  /* out of memory */
        for (idx_t i = 0; i < new_len; ++i) cache[i] = (seq_t)0;
        cache_len = new_len;
    } else if (cache_len <= n) {
        /* grow the cache to hold up to n */
        size_t new_len = (size_t)(n + 1);
        seq_t *tmp = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
        if (!tmp) abort();  /* out of memory */
        cache = tmp;
        for (idx_t i = cache_len; i < new_len; ++i) cache[i] = (seq_t)0;
        cache_len = new_len;
    }
}

/* Initialise base values if this is the first call. */
static void
init_base(void)
{
    if (max_computed != (idx_t)-1) return;   /* already initialised */
    cache[0] = (seq_t)0;
    cache[1] = (seq_t)3;
    cache[2] = (seq_t)10;
    cache[3] = (seq_t)36;
    cache[4] = (seq_t)118;
    max_computed = 4;
}

/* Compute missing terms up to index n. */
static void
compute_to(idx_t n)
{
    init_base();
    if (n <= max_computed) return;

    for (idx_t i = max_computed + 1; i <= n; ++i) {
        /* a(i) = 3*a(i-1) + 2*a(i-2) - 3*a(i-3) - a(i-4)   (i ≥ 5) */
        cache[i] = 3 * cache[i - 1]
                 + 2 * cache[i - 2]
                 - 3 * cache[i - 3]
                 -   cache[i - 4];
    }
    max_computed = n;
}

seq_t a(idx_t n)
{
    ensure_cache(n);
    compute_to(n);
    return cache[n];
}
