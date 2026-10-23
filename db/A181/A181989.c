/*
 * A181989  Number of independent sets of nodes in graph C_5 X P_n (n >= 0).
 * https://oeis.org/A181989
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 7*a(n-1) + 5*a(n-2) - a(n-3), a(0)=1, a(1)=11, a(2)=81
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 21, u128 n <= 43,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Computes the n-th term of OEIS sequence A181989.
   The function keeps a static cache so that calls in any order
   are efficient.  All arithmetic is performed with the type
   seq_t, so the implementation works for 8‑, 16‑, 32‑, 64‑, and
   128‑bit unsigned integers as well as arbitrary‑precision builds. */
seq_t a(idx_t n)
{
    /* static cache: cache[i] holds a(i) for 0 <= i < cache_size */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;   /* number of elements stored */
    static idx_t cache_cap  = 0;   /* allocated capacity */

    /* Prepare initial cache if empty */
    if (cache_size == 0) {
        /* Allocate space for the three known initial terms */
        cache_cap  = 3;
        cache_size = 3;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) return (seq_t)0;            /* allocation failure → 0 */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)11;
        cache[2] = (seq_t)81;
    }

    /* If the requested index is already cached, return it */
    if (n < cache_size) {
        return cache[n];
    }

    /* Grow the cache if necessary */
    if (n >= cache_cap) {
        idx_t new_cap = cache_cap > 0 ? cache_cap : 1;
        while (new_cap <= n) new_cap *= 2;      /* exponential growth */
        seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!new_cache) return (seq_t)0;        /* allocation failure → 0 */
        cache     = new_cache;
        cache_cap = new_cap;
    }

    /* Compute missing terms up to n */
    for (idx_t i = cache_size; i <= n; ++i) {
        /* a(i) = 7*a(i-1) + 5*a(i-2) - a(i-3) */
        seq_t term1 = (seq_t)7 * cache[i - 1];
        seq_t term2 = (seq_t)5 * cache[i - 2];
        cache[i] = term1 + term2 - cache[i - 3];
    }

    cache_size = n + 1;    /* we now have terms 0..n */
    return cache[n];
}
