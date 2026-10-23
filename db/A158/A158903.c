/*
 * A158903  Numerator of Hermite(n, 2/3).
 * https://oeis.org/A158903
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, frac
 * Formula:   a(n) = 4*a(n-1) - 18*(n-1)*a(n-2) with a(0)=1, a(1)=4
 * Range:     i8 n <= 2, i16 n <= 5, i32 n <= 9, i64 n <= 18, i128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h> /* malloc, realloc, abort */

/* Memoisation array: holds a(0)..a(current_max). */
static seq_t *cache = NULL;
static idx_t cache_len = 0;   /* number of values stored so far */
static size_t cache_cap = 0;  /* capacity of the allocated array */

/* Ensure the cache contains values up to a(n). */
static void ensure_cache(idx_t n)
{
    if (cache_len > n) return;            /* already have it */

    /* Grow the array if necessary. */
    size_t target = (size_t)(n + 1);
    if (cache_cap < target) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 2;
        if (new_cap < target) new_cap = target;
        seq_t *tmp = (seq_t *)realloc(cache, sizeof(seq_t) * new_cap);
        if (!tmp) abort();                /* allocation failure */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Compute missing terms. */
    while (cache_len <= n) {
        if (cache_len == 0) {
            cache[0] = (seq_t)1;
        } else if (cache_len == 1) {
            cache[1] = (seq_t)4;
        } else {
            /* a[n] = 4 * a[n-1] - 18 * (n-1) * a[n-2] */
            idx_t i = cache_len; /* current index */
            seq_t term = (seq_t)18 * (seq_t)(i - 1) * cache[i - 2];
            seq_t next = cache[i - 1] * (seq_t)4 - term;
            cache[i] = next;
        }
        ++cache_len;
    }
}

seq_t a(idx_t n)
{
    ensure_cache(n);
    return cache[n];
}
