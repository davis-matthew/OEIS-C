/*
 * A078026  Expansion of (1-x)/(1-x^2-2*x^3).
 * https://oeis.org/A078026
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = a(n-2) + 2*a(n-3) (n ≥ 3), a(0)=1, a(1)=-1, a(2)=1
 * Range:     i8 n <= 16, i16 n <= 29, i32 n <= 56, i64 n <= 108,
 *            i128 n <= 214, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Expansion of (1-x)/(1-x^2-2*x^3).  The function memoizes values so
   subsequent calls are answered in O(1). */

#include <stdlib.h>
#include <stdint.h>

/* Lazily grown cache of computed terms */
static seq_t *cache = NULL;        /* array of a(n) */
static size_t cache_cap = 0;       /* number of allocated slots */
static idx_t  last = (idx_t)(-1);  /* largest n for which a(n) is known */

/* Ensure cache holds at least indices 0..n */
static void ensure_size(size_t n)
{
    if (n < cache_cap) return;
    size_t new_cap = cache_cap > 0 ? cache_cap : 4;
    while (new_cap <= n) new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) abort();                 /* memory exhausted */
    cache = tmp;
    cache_cap = new_cap;
}

/* Initialise cache with first three terms */
static void init_cache(void)
{
    cache = (seq_t *)malloc(3 * sizeof(seq_t));
    if (!cache) abort();
    cache_cap = 3;
    cache[0] = (seq_t)1;
    cache[1] = (seq_t)-1;
    cache[2] = (seq_t)1;
    last = 2;
}

seq_t a(idx_t n)
{
    /* Ensure cache exists */
    if (!cache) init_cache();

    /* If requested index is beyond what we have, compute more terms */
    if (n > last) {
        size_t N = (size_t)n;            /* safe for array indexing   */
        ensure_size(N);
        for (idx_t k = last + 1; k <= n; ++k) {
            /* a(k) = a(k-2) + 2*a(k-3) */
            seq_t term = cache[(size_t)(k - 2)];
            term += cache[(size_t)(k - 3)] * (seq_t)2;
            cache[(size_t)k] = term;
        }
        last = n;
    }

    return cache[(size_t)n];
}
