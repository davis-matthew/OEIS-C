/*
 * A094667  Number of (s(0), s(1), ..., s(2n+1)) such that 0 < s(i) < 10 and
 *          |s(i) - s(i-1)| = 1 for i = 1,2,...,2n+1, s(0) = 1, s(2n+1) = 4.
 * https://oeis.org/A094667
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(1) = 1, a(2) = 4, a(3) = 14, a(4) = 48 a(n) = 8*a(n-1) -
 *            21*a(n-2) + 20*a(n-3) - 5*a(n-4) for n >= 5 The sequence counts
 *            walks of length 2n+1 on the 1‑9 path graph, starting at 1 and
 *            ending at 4. The recurrence yields non‑negative intermediate
 *            values for all n ≥ 5, so unsigned arithmetic is saf...
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 18, u64 n <= 35, u128 n <= 70,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Static cache: index 0 unused, cache[1..max_n] holds computed terms. */
    static seq_t *cache = NULL;
    static idx_t max_n = 0;

    if (max_n == 0) {
        /* Initialise for n = 1 .. 4.  Allocate space for indices 0..4. */
        cache = (seq_t *)malloc(5 * sizeof(seq_t));
        if (!cache) return (seq_t)0;          /* Out of memory: return zero. */
        cache[0] = 0;                         /* unused sentinel */
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)4;
        cache[3] = (seq_t)14;
        cache[4] = (seq_t)48;
        max_n = 4;
    }

    if (n <= max_n) {
        return cache[n];
    }

    /* Grow cache to accommodate n. */
    idx_t new_len = n + 1;          /* we need indices 0 .. n */
    seq_t *new_cache = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
    if (!new_cache) {
        /* Realloc failed, keep existing cache and return zero. */
        return (seq_t)0;
    }
    cache = new_cache;

    /* Compute new terms up to n. */
    for (idx_t i = max_n + 1; i <= n; ++i) {
        seq_t term = cache[i - 1] * (seq_t)8;
        term -= cache[i - 2] * (seq_t)21;
        term += cache[i - 3] * (seq_t)20;
        term -= cache[i - 4] * (seq_t)5;
        cache[i] = term;
    }

    max_n = n;
    return cache[n];
}
