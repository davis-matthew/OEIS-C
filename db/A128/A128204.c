/*
 * A128204  a(0) = 0; a(n) = a(n-1) - (2n-1) if that number is positive and
 *          not already in the sequence, otherwise a(n) = a(n-1) + (2n-1).
 * https://oeis.org/A128204
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(0) = 0; a(n) = a(n-1) - (2n-1) if the result is positive and
 *            not already present, otherwise a(n) = a(n-1) + (2n-1)
 * Range:     u8 n <= 43, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>
#include "../utils/seq_realloc.h"

/* Internal cache of the sequence values. */
static seq_t *a_cache     = NULL;
static size_t a_cache_cap = 0;   /* allocated capacity (bytes of seq_t) */
static idx_t  a_cache_n   = 0;   /* number of valid elements (max index+1) */

/* Make sure the cache can hold indices 0 .. needed. */
static void ensure_capacity(idx_t needed)
{
    if ((idx_t)a_cache_cap > needed)
        return;
    size_t new_cap = a_cache_cap ? a_cache_cap : 1;
    while ((idx_t)new_cap <= needed)
        new_cap <<= 1;
    a_cache = seq_realloc(a_cache, a_cache_cap, new_cap);
    a_cache_cap = new_cap;
}

/* Test whether value `val` is already stored in a_cache[0 .. limit-1]. */
static bool already_present(seq_t val, idx_t limit)
{
    for (idx_t i = 0; i < limit; ++i)
        if (a_cache[i] == val)
            return true;
    return false;
}

seq_t a(idx_t n)
{
    if (n < a_cache_n)             /* already computed */
        return a_cache[n];

    /* Grow cache to fit indices up to n. */
    ensure_capacity(n);

    if (a_cache_n == 0) {          /* initialise a(0) */
        a_cache[0] = 0;
        a_cache_n = 1;
    }

    for (idx_t k = a_cache_n; k <= n; ++k) {
        seq_t prev = a_cache[k - 1];
        seq_t odd  = (seq_t)((idx_t)2 * (idx_t)k - (seq_t)1); /* 2k-1 */
        seq_t cand;

        if (prev > odd) {
            cand = prev - odd;
            if (cand > (seq_t)0 && !already_present(cand, k)) {
                a_cache[k] = cand;   /* subtraction path */
                continue;
            }
        }
        a_cache[k] = prev + odd;      /* addition path */
    }

    a_cache_n = n + 1;
    return a_cache[n];
}
