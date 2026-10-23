/*
 * A192874  Constant term in the reduction by (x^2 -> x + 1) of the polynomial
 *          p(n,x) given in Comments.
 * https://oeis.org/A192874
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*a(n-1) + 6*a(n-2) - 5*a(n-3) - 6*a(n-4) + 4*a(n-5),
 *            a(0)=1, a(1)=0, a(2)=4, a(3)=6, a(4)=26
 * Range:     u8 n <= 6, u16 n <= 10, u32 n <= 20, u64 n <= 39, u128 n <= 76,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>   /* malloc, realloc */

/* Initial values for n = 0..4 */
static const seq_t init_vals[5] = {1, 0, 4, 6, 26};

/* Cache for previously computed values */
static seq_t *cache = NULL;
static size_t cache_cap = 0;   /* allocated size */
static size_t cache_len = 0;   /* number of valid entries (index+1) */

/* Ensure that the cache has space for at least n+1 elements */
static void ensure_cache_size(idx_t n)
{
    /* If already large enough, nothing to do */
    if ((size_t)n + 1 <= cache_cap)
        return;

    /* Determine new capacity (double until large enough) */
    size_t new_cap = cache_cap ? cache_cap : 8;
    while ((size_t)n + 1 > new_cap)
        new_cap <<= 1;

    /* Reallocate */
    seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!new_cache) {
        /* Allocation failed; abort (no error handling as per requirements) */
        abort();
    }
    cache = new_cache;
    cache_cap = new_cap;
}

/* Function to compute a(n) for any n >= 0 */
seq_t a(idx_t n)
{
    /* Direct return for the first five terms */
    if (n < 5)
        return init_vals[(size_t)n];

    /* Initialise cache on first use */
    if (!cache) {
        ensure_cache_size(4);   /* at least space for 0..4 */
        for (size_t i = 0; i < 5; ++i)
            cache[i] = init_vals[i];
        cache_len = 5;
    }

    /* Extend cache if necessary */
    ensure_cache_size(n);

    /* Compute missing terms up to n */
    for (idx_t i = (idx_t)cache_len; i <= n; ++i) {
        seq_t val = cache[i-1];
        val *= 2u;                       /* 2*a(i-1) */

        seq_t tmp = cache[i-2];
        tmp *= 6u;                       /* 6*a(i-2) */
        val += tmp;

        tmp = cache[i-3];
        tmp *= 5u;                       /* 5*a(i-3) */
        val -= tmp;

        tmp = cache[i-4];
        tmp *= 6u;                       /* 6*a(i-4) */
        val -= tmp;

        tmp = cache[i-5];
        tmp *= 4u;                       /* 4*a(i-5) */
        val += tmp;

        cache[i] = val;
    }

    cache_len = (size_t)n + 1;
    return cache[(size_t)n];
}
