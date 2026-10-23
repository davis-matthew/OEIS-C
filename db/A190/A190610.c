/*
 * A190610  a(1)=a(2)=1; thereafter a(n) = a(ceiling(n/2))-a(floor(n/2)).
 * https://oeis.org/A190610
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(1)=a(2)=1; for n>2 a(n)=a(ceil(n/2))-a(floor(n/2))
 * Range:     i8 n <= 101, i16 n <= 101, i32 n <= 101, i64 n <= 101,
 *            i128 n <= 101, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

/* Static cache for memoization */
static seq_t *cache   = NULL;
static bool   *seen   = NULL;
static size_t  cache_cap = 0;

/* Ensure the cache can hold values up to n */
static void ensure_cache(idx_t n)
{
    if (n <= cache_cap)
        return;

    size_t old_cap = cache_cap;
    /* Grow by at least 1, avoid zero */
    size_t new_cap = n * 2 + 1;
    if (new_cap == 0)
        new_cap = 1;

    cache = (seq_t *)realloc(cache, (new_cap + 1) * sizeof(seq_t));
    if (!cache)
        exit(1); /* OOM – abort */

    seen = (bool *)realloc(seen, (new_cap + 1) * sizeof(bool));
    if (!seen)
        exit(1); /* OOM – abort */

    /* Mark new slots as uncomputed */
    for (size_t i = old_cap + 1; i <= new_cap; ++i)
        seen[i] = false;

    cache_cap = new_cap;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n <= 2)
        return (seq_t)1;   /* base cases */

    if (!cache)
        ensure_cache(2);

    if (n > cache_cap)
        ensure_cache(n);

    if (seen[n])
        return cache[n];

    idx_t ceil_n = (n + 1) / 2;
    idx_t floor_n = n / 2;

    seq_t val = a(ceil_n) - a(floor_n);

    if (n <= cache_cap) {
        cache[n] = val;
        seen[n]  = true;
    }
    return val;
}
