/*
 * A157877  Expansion of (1-x)*x/(x^2-30*x+1).
 * https://oeis.org/A157877
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 30*a(n-1) - a(n-2), with a(1) = 1, a(2) = 29 Uses
 *            memoization to answer any query in non‑increasing order of n
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 7, u64 n <= 14, u128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Static cache for values of the sequence. */
static seq_t *a_cache = NULL;
static idx_t  a_cache_len = 0;   /* number of elements currently stored */
static idx_t  a_cache_cap = 0;   /* allocated capacity */

/* Grow the cache so that it can hold at least 'needed' elements. */
static void grow_a_cache(idx_t needed)
{
    if (needed <= a_cache_cap) return;

    idx_t new_cap = a_cache_cap ? a_cache_cap * 2 : 4;
    while (new_cap < needed) new_cap *= 2;

#ifdef __cplusplus
    /* In C++ use the operator new[] to construct objects. */
    seq_t *new_arr = new seq_t[new_cap];
    for (idx_t i = 0; i < a_cache_len; ++i) new_arr[i] = a_cache[i];
    delete[] a_cache;
    a_cache = new_arr;
#else
    /* In C realloc is sufficient because seq_t is an integer type. */
    seq_t *new_arr = realloc(a_cache, new_cap * sizeof(seq_t));
    if (!new_arr) abort();          /* allocation failed */
    a_cache = new_arr;
#endif

    a_cache_cap = new_cap;
}

/* Return a(n) for any n >= 1. */
seq_t a(idx_t n)
{
    /* a(1) is the first term. Any n < 1 returns 0. */
    if (n == 0) return 0;

    if (a_cache_len < n) {
        grow_a_cache(n);

        /* initialise the cache if it is empty. */
        if (a_cache_len == 0) {
            a_cache[0] = (seq_t)1;          /* a(1) */
            a_cache_len = 1;
        }
        if (a_cache_len < 2) {
            a_cache[1] = (seq_t)29;         /* a(2) */
            a_cache_len = 2;
        }
        /* compute missing terms using the recurrence. */
        for (idx_t i = a_cache_len; i < n; ++i) {
            a_cache[i] = ((seq_t)30) * a_cache[i-1] - a_cache[i-2];
        }
        a_cache_len = n;
    }

    return a_cache[n-1];
}
