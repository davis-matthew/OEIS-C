/*
 * A170207  Number of reduced words of length n in Coxeter group on 6
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^40 = I.
 * https://oeis.org/A170207
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   * a(0) = 1 * for n >= 1: * a(n) = 4 * Σ_{k=1}^{39} a(n-k) – 10 *
 *            a(n-40) + p(n) * where * p(0) = 1 * p(1..39) = 2 * p(40) = 1 *
 *            p(other) = 0 * (This follows from the rational generating
 *            function of A170207.)
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 13, u64 n <= 21, u128 n <= 21,
 *            big any n
 * Helpers:   seq_alloc_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include "../utils/seq_alloc_array.h"

/* In case a memory allocator is supplied by the test harness,
 * use it.  Otherwise fall back to a simple malloc wrapper. */
#ifndef seq_alloc_array

#endif

/* Cache of computed values.  Allocated lazily and enlarged by powers of two. */
static seq_t *cache = NULL;      /* array of a(n) */
static idx_t  cache_size = 0;    /* number of filled entries */
static idx_t  cache_cap  = 0;    /* allocated capacity */

/* Ensure capacity for indices up to 'need'.  The array is grown to
 * the smallest power of two that is > need.  The old memory block
 * is intentionally leaked; this is safe for a short‑lived program
 * and avoids constructor/destructor issues for arbitrary‑precision
 * types. */
static void
ensure_capacity(idx_t need)
{
    if (need < cache_cap)
        return;

    idx_t new_cap = cache_cap ? cache_cap << 1 : 1;
    while (new_cap <= need)
        new_cap <<= 1;

    seq_t *new_cache = seq_alloc_array((size_t)new_cap);

    for (idx_t i = 0; i < cache_size; ++i)
        new_cache[i] = cache[i];

    cache = new_cache;
    cache_cap = new_cap;
}

/* Main sequence function. */
seq_t
a(idx_t n)
{
    /* Initialise cache if this is the first call. */
    if (cache_size == 0) {
        ensure_capacity(0);
        cache[0] = (seq_t)1;
        cache_size = 1;
    }

    /* Compute successive values until we reach the requested index. */
    while (cache_size <= n) {
        idx_t idx = cache_size;          /* compute a(idx) */
        seq_t sum = (seq_t)0;

        /* Sum the 39 preceding terms. */
        idx_t start = (idx > 39) ? idx - 39 : 0;
        for (idx_t j = start; j < idx; ++j)
            sum += cache[j];

        seq_t val = (seq_t)4 * sum;

        /* Subtract 10 times the term 40 positions back, if it exists. */
        if (idx >= 40)
            val -= (seq_t)10 * cache[idx - 40];

        /* Add the p(n) contribution. */
        seq_t p = (seq_t)0;
        if (idx == 0)
            p = (seq_t)1;
        else if (idx <= 39)
            p = (seq_t)2;
        else if (idx == 40)
            p = (seq_t)1;

        val += p;

        /* Store the computed value. */
        ensure_capacity(idx);
        cache[cache_size++] = val;
    }

    return cache[n];
}
