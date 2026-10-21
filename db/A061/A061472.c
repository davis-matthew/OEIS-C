/*
 * A061472  First (leftmost) digit - second digit + third digit - fourth digit
 *          .... = 3.
 * https://oeis.org/A061472
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 21, u16 n <= 4166, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   alt_sum_is_three
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A061472: the n‑th positive integer whose alternating sum of decimal
   digits (starting with the most significant digit positive) equals 3.
   Computation: iterate over natural numbers in increasing order and keep
   those that satisfy the condition.  Results are cached for repeated queries. */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/alt_sum_is_three.h"

/* External types supplied by the build environment */
extern seq_t _dummy;  /* placeholder to force inclusion of seq_t type */

/* Cache for already computed terms */
static seq_t *term_cache = NULL;   /* array of terms */
static idx_t  cache_size   = 0;    /* number of stored terms */
static idx_t  cache_cap    = 0;    /* allocated capacity */

/* Ensure the cache has enough space for 'needed' terms */
static void ensure_capacity(idx_t needed)
{
    if (needed <= cache_cap) return;
    idx_t new_cap = cache_cap ? cache_cap * 2 : 8;
    while (new_cap < needed) new_cap *= 2;
    term_cache = (seq_t *)realloc(term_cache, new_cap * sizeof(seq_t));
    cache_cap  = new_cap;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* offset starts at 1; behaviour below irrelevant */

    /* Make sure we have room for the requested term */
    ensure_capacity(n);

    /* If already known, return it directly */
    if (n <= cache_size) return term_cache[n-1];

    /* Generate further terms until reaching the n‑th term */
    seq_t candidate = (cache_size == 0) ? 1 : term_cache[cache_size-1] + 1;
    while (cache_size < n)
    {
        if (alt_sum_is_three(candidate))
            term_cache[cache_size++] = candidate;
        candidate++;
    }
    return term_cache[n-1];
}
