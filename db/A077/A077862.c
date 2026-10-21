/*
 * A077862  Expansion of (1-x)^(-1)/(1-2*x+2*x^2+2*x^3).
 * https://oeis.org/A077862
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = 3*a(n-1) - 4*a(n-2) + 2*a(n-4), with initial values
 *            a(0)=1, a(1)=3, a(2)=5, a(3)=3
 * Range:     i8 n <= 7, i16 n <= 16, i32 n <= 35, i64 n <= 35, i128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function memoises values so that any order of calls is efficient. */

#include <stdlib.h>
#include <string.h>

static seq_t *cache = NULL;     /* memoisation array */
static size_t cache_cap = 0;    /* current capacity of `cache` */
static size_t cached_len = 0;   /* number of entries that have been initialized */

/* Resize the cache if necessary. */
static void ensure_capacity(size_t needed)
{
    if (cache_cap >= needed)
        return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 16;
    while (new_cap < needed)
        new_cap *= 2;
    seq_t *new_cache = (seq_t *)malloc(new_cap * sizeof(seq_t));
    if (cache) {
        memcpy(new_cache, cache, cache_cap * sizeof(seq_t));
        free(cache);
    }
    cache = new_cache;
    cache_cap = new_cap;
}

/* Initialise the first four terms once. */
static void init_terms(void)
{
    size_t init_size = 4;
    ensure_capacity(init_size);
    cache[0] = (seq_t)1;
    cache[1] = (seq_t)3;
    cache[2] = (seq_t)5;
    cache[3] = (seq_t)3;
    cached_len = init_size;
}

seq_t a(idx_t n)
{
    /* Handle trivial small indices directly. */
    if (n < 4)
        return (n == 0) ? (seq_t)1 :
               (n == 1) ? (seq_t)3 :
               (n == 2) ? (seq_t)5 :
                          (seq_t)3;

    /* Ensure cache exists and contains initial terms. */
    if (cache == NULL)
        init_terms();

    /* If the requested value is already computed, return it. */
    if ((size_t)n < cached_len)
        return cache[(size_t)n];

    /* Grow the cache to accommodate indices up to n. */
    ensure_capacity((size_t)n + 1);

    /* Compute missing terms using the recurrence. */
    for (size_t k = cached_len; k <= (size_t)n; ++k) {
        /* All indices k-1, k-2, k-4 are valid because k >= 4. */
        seq_t t1 = cache[k - 1];
        seq_t t2 = cache[k - 2];
        seq_t t4 = cache[k - 4];
        cache[k] = (seq_t)3 * t1 - (seq_t)4 * t2 + (seq_t)2 * t4;
    }
    cached_len = (size_t)n + 1;
    return cache[(size_t)n];
}
