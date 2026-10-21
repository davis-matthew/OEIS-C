/*
 * A057682  a(n) = Sum_{j=0..floor(n/3)} (-1)^j*binomial(n,3*j+1).
 * https://oeis.org/A057682
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = 3*a(n-1) - 3*a(n-2) for n >= 3 a(0) = 0, a(1) = 1, a(2) =
 *            2
 * Range:     i8 n <= 11, i16 n <= 19, i32 n <= 41, i64 n <= 80, i128 n <= 162,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* The function computes OEIS sequence A057682.
   It caches previously computed values so that calls in
   arbitrary order (with n up to ~10 000) are fast.  */
seq_t a(idx_t n)
{
    /* Handle the first three terms directly. */
    if (n == 0) return (seq_t)0;
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)2;

    /* Static cache of computed values. */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;   /* number of values stored (>=3 after init) */
    static idx_t cache_alloc = 0;  /* allocated capacity */

    /* Initialise the cache the first time. */
    if (cache == NULL) {
        cache_alloc = 16;                     /* initial capacity */
        cache = (seq_t *)malloc(sizeof(seq_t) * cache_alloc);
        if (!cache) return (seq_t)0;          /* allocation failure – unlikely */
        cache[0] = (seq_t)0;
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)2;
        cache_size = 3;
    }

    /* Grow cache if needed. */
    while (n >= cache_alloc) {
        idx_t new_alloc = cache_alloc * 2;
        seq_t *new_cache = (seq_t *)realloc(cache, sizeof(seq_t) * new_alloc);
        if (!new_cache) break; /* allocation failure – keep old cache */
        cache = new_cache;
        cache_alloc = new_alloc;
    }

    /* Compute missing values up to n. */
    while (cache_size <= n) {
        /* Use the recurrence: a(i) = 3*a(i-1) - 3*a(i-2) */
        seq_t val = (seq_t)3 * cache[cache_size - 1] - (seq_t)3 * cache[cache_size - 2];
        cache[cache_size] = val;
        ++cache_size;
    }

    return cache[n];
}
