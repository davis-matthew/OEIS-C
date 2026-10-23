/*
 * A171068  Expansion of g.f. x*(1 - x)*(1 + x)/(1 - x - 11*x^2 - x^3 + x^4).
 * https://oeis.org/A171068
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 17, u64 n <= 34, u128 n <= 66,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A171068
   Formula: a(n) = a(n‑1) + 11*a(n‑2) + a(n‑3) – a(n‑4)
   Initial values: a(0) = 0, a(1) = 1, a(2) = 1, a(3) = 11
   Compute with an O(n) recurrence, memoising the results. */

#include <stdlib.h>  /* malloc, realloc */

static seq_t *cache = NULL;   /* dynamic array of computed terms */
static size_t cache_len = 0;  /* number of stored values */

/* Return the n‑th term of the sequence.  n is unsigned 64‑bit. */
seq_t a(idx_t n)
{
    /* Ensure the cache contains at least the first four values. */
    if (cache_len == 0) {
        cache_len = 4;
        cache = (seq_t *)malloc(cache_len * sizeof(seq_t));
        cache[0] = (seq_t)0;
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)1;
        cache[3] = (seq_t)11;
    }

    /* If the requested term is already computed, return it immediately. */
    if ((size_t)n < cache_len)
        return cache[(size_t)n];

    /* Expand the cache to hold up to n. */
    size_t new_len = (size_t)n + 1;
    seq_t *tmp = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
    if (!tmp) {
        /* Allocation failed; return 0 to signal error (caller should not call with impossible indices). */
        return (seq_t)0;
    }
    cache = tmp;
    size_t old_len = cache_len;
    cache_len = new_len;

    /* Compute missing terms using the recurrence. */
    for (idx_t i = old_len; i <= n; ++i) {
        seq_t v1 = cache[(size_t)(i - 1)];
        seq_t v2 = cache[(size_t)(i - 2)];
        seq_t v3 = cache[(size_t)(i - 3)];
        seq_t v4 = cache[(size_t)(i - 4)];
        cache[(size_t)i] = v1 + (seq_t)11 * v2 + v3 - v4;
    }

    return cache[(size_t)n];
}
