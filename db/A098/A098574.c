/*
 * A098574  a(n) = Sum_{k=0..floor(n/7)} C(n-5*k,2*k).
 * https://oeis.org/A098574
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 19, u16 n <= 35, u32 n <= 67, u64 n <= 130,
 *            u128 n <= 257, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Sequence A098574
 *  Definition:  a(n) = Σ_{k=0..⌊n/7⌋} binom(n‑5k, 2k)
 *
 *  Recurrence (valid for n ≥ 7):
 *      a(0) … a(6) = 1
 *      a(n) = 2*a(n‑1) – a(n‑2) + a(n‑7)
 *
 *  The implementation uses a static cache and computes terms on demand.
 *  All arithmetic is performed with the type seq_t, so it works for
 *  built‑in unsigned integer types as well as arbitrary‑precision
 *  integer classes that support +, -, and * by integer constants.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Static cache of computed terms */
    static seq_t *cache   = NULL;   /* array of a(i) for 0 ≤ i < cache_size */
    static idx_t cache_size = 0;    /* number of elements allocated */

    /* Initialise the cache with the seven fixed initial values */
    if (cache == NULL) {
        cache_size = 7;                     /* indices 0..6 inclusive */
        cache = (seq_t *)calloc((size_t)cache_size, sizeof(seq_t));
        /* All entries are already zero; set the seven initial terms to 1 */
        for (idx_t i = 0; i < 7; ++i) cache[i] = (seq_t)1;
    }

    /* If the requested index is already cached, just return it */
    if (n < cache_size) return cache[n];

    /* Extend the cache up to the requested index */
    size_t new_size = (size_t)(n + 1);
    seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
    if (tmp == NULL) {
        /* Allocation failed – fallback to computing only the required term
           without further memoisation (unlikely for n ≤ 10000). */
        for (idx_t i = cache_size; i <= n; ++i) {
            if (i < 7) {
                /* already 1 */
                continue;
            }
            seq_t val = cache[i - 1] + cache[i - 1];  /* 2*a(i‑1) */
            val -= cache[i - 2];                      /* – a(i‑2) */
            val += cache[i - 7];                      /* + a(i‑7) */
            cache[i] = val;
        }
        cache_size = n + 1;
        return cache[n];
    }
    cache = tmp;
    /* Old elements [0..cache_size-1] remain unchanged. */
    for (idx_t i = cache_size; i <= n; ++i) {
        if (i < 7) {
            /* Already set to 1 during initialisation. */
            continue;
        }
        seq_t val = cache[i - 1] + cache[i - 1];  /* 2*a(i‑1) */
        val -= cache[i - 2];                      /* – a(i‑2) */
        val += cache[i - 7];                      /* + a(i‑7) */
        cache[i] = val;
    }
    cache_size = n + 1;
    return cache[n];
}
