/*
 * A092499  Chebyshev polynomials S(n-1,21) with Diophantine property.
 * https://oeis.org/A092499
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 15, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: A092499  
   Formula: a(0) = 0, a(1) = 1, a(n) = 21*a(n-1) - a(n-2)  (n >= 2)   */

#include <stdlib.h>

static seq_t *a_cache = NULL;   /* memoisation table */
static idx_t  cache_len = 0;    /* number of entries stored */

/* Ensure the cache contains values up to index n (inclusive) */
static void
ensure_cache(idx_t n)
{
    if (cache_len > n)          /* already computed */
        return;

    /* Extend the cache */
    size_t new_len = (size_t)(n + 1);
    seq_t *new_ptr = (seq_t *)realloc(a_cache, new_len * sizeof(seq_t));
    a_cache = new_ptr;

    /* Initialise new portions */
    if (cache_len == 0) {
        a_cache[0] = (seq_t)0;
        if (new_len > 1)
            a_cache[1] = (seq_t)1;
    } else if (cache_len == 1) {
        a_cache[1] = (seq_t)1;
    }

    /* Compute missing terms using the recurrence */
    for (idx_t i = cache_len; i <= n; ++i) {
        if (i == 0)
            continue;      /* already set */
        if (i == 1)
            continue;      /* already set */
        /* a(i) = 21*a(i-1) - a(i-2) */
        a_cache[i] = (seq_t)21 * a_cache[i - 1] - a_cache[i - 2];
    }

    cache_len = new_len;
}

seq_t
a(idx_t n)
{
    /* The sequence is defined for n >= 0; behaviour for negative is undefined */
    if (n == (idx_t)0)
        return (seq_t)0;
    if (n == (idx_t)1)
        return (seq_t)1;

    ensure_cache(n);
    return a_cache[n];
}
