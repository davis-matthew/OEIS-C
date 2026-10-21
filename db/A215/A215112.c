/*
 * A215112  a(n) = -2*a(n-1) + a(n-2) + a(n-3) with a(0)=a(1)=-1, a(2)=1.
 * https://oeis.org/A215112
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = -2*a(n-1) + a(n-2) + a(n-3) a(0) = -1, a(1) = -1, a(2) =
 *            1
 * Range:     i8 n <= 7, i16 n <= 14, i32 n <= 27, i64 n <= 30, i128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>   /* malloc, realloc, free */

static seq_t *cache = NULL;      /* dynamic cache of computed terms */
static idx_t  last_comp = 0;     /* highest index that has been computed so far */
static size_t cache_cap = 0;     /* current capacity of the cache */

/* Ensure the cache can hold indices up to n (inclusive). */
static void
ensure_cache(idx_t n)
{
    if (!cache) {
        /* first allocation: store the first three base values */
        cache_cap = (size_t)(n + 1 > 4 ? n + 1 : 4);
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        cache[0] = -1;
        cache[1] = -1;
        cache[2] = 1;
        last_comp = 2;
    } else if (cache_cap <= (size_t)n) {
        size_t new_cap = cache_cap * 2;
        while (new_cap <= (size_t)n) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cache = tmp;
        cache_cap = new_cap;
    }
}

/* Compute a(n) according to the recurrence.  The function is memoised
   and can be called in any order. */
seq_t a(idx_t n)
{
    /* Handle the first three indices explicitly. */
    if (n == 0) return -1;
    if (n == 1) return -1;
    if (n == 2) return 1;

    /* Ensure the cache is large enough and contains the base terms. */
    ensure_cache(n);

    /* Compute missing terms up to n. */
    while (last_comp < n) {
        /* The next index to compute is last_comp + 1. */
        idx_t next = last_comp + 1;

        /* Calculate the recurrence value. */
        seq_t val = -2 * cache[last_comp] + cache[last_comp - 1] + cache[last_comp - 2];

        /* Store and advance the last computed index. */
        cache[next] = val;
        last_comp = next;
    }

    return cache[n];
}
