/*
 * A167603  Expansion of 1/(1 + 837*x + 277760*x^2 + 83891456*x^3 +
 *          7809531904*x^4).
 * https://oeis.org/A167603
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n+4) + 837*a(n+3) + 277760*a(n+2) + 83891456*a(n+1) +
 *            7809531904*a(n) = 0 Initial values: a(0) = 1 a(1) = -837 a(2) =
 *            422809 a(3) = -205297469
 * Range:     i8 n <= 0, i16 n <= 1, i32 n <= 3, i64 n <= 6, i128 n <= 13,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>

/* Cached sequence values */
static seq_t *cache = NULL;        /* array holding computed terms       */
static size_t cache_cap = 0;       /* current capacity (number of slots) */
static idx_t  last_computed = 3;   /* highest index that has a value */

/* Ensure that the cache can hold indices up to n */
static void ensure_cache(idx_t n)
{
    if ((size_t)n < cache_cap)
        return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 8;
    while (new_cap <= (size_t)n)
        new_cap <<= 1;
    seq_t *new_arr = (seq_t *)malloc(new_cap * sizeof(seq_t));
    if (!new_arr)
        exit(1); /* Allocation failure */
    if (cache) {
        memcpy(new_arr, cache, cache_cap * sizeof(seq_t));
        free(cache);
    }
    cache = new_arr;
    cache_cap = new_cap;
}

/* Initialise the first four terms once */
static void init_base(void)
{
    static int init_done = 0;
    if (init_done)
        return;
    ensure_cache(3);
    cache[0] = (seq_t)1;
    cache[1] = (seq_t)-837;
    cache[2] = (seq_t)422809;
    cache[3] = (seq_t)-205297469;
    init_done = 1;
}

seq_t a(idx_t n)
{
    init_base();                     /* make sure indices 0..3 exist */

    if (n <= last_computed)         /* already computed */
        return cache[n];

    /* Ensure we have enough space in the cache */
    ensure_cache(n);

    /* Compute terms up to n */
    for (idx_t i = last_computed + 1; i <= n; ++i) {
        seq_t term =
            cache[i-1] * (seq_t)837
          + cache[i-2] * (seq_t)277760
          + cache[i-3] * (seq_t)83891456LL
          + cache[i-4] * (seq_t)7809531904ULL;
        cache[i] = -term;
    }
    last_computed = n;
    return cache[n];
}
