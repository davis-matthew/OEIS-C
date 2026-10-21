/*
 * A216836  Numbers n such that sum of decimal digits of n divides phi(n).
 * https://oeis.org/A216836
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 80, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   calc_phi, digit_sum
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: Numbers n such that the sum of decimal digits of n divides φ(n).
   Compute by scanning integers and caching the results. */
#include <stdlib.h>
#include "../utils/digit_sum.h"
#include "../utils/calc_phi.h"

/* Static cache for the terms found so far. */
static seq_t *cache = NULL;      /* cached terms */
static size_t cache_len = 0;     /* number of cached terms */
static size_t cache_cap = 0;     /* allocated capacity */
static idx_t  next_candidate = 1;/* next integer to test */

/* Maximum value representable by seq_t. */
static const seq_t MAX_SEQ_VAL = ~(seq_t)0;

/* Append a new term to the cache, expanding capacity if needed. */
static void append_term(seq_t val)
{
    if (cache_len == cache_cap) {
        size_t new_cap = cache_cap ? cache_cap << 1 : 128;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return;          /* realloc failed – ignore, caller will
                                       not get more terms */
        cache = tmp;
        cache_cap = new_cap;
    }
    cache[cache_len++] = val;
}

/* Return the n‑th term of the sequence, with a(1) being the first term. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* convention for n=0 not used */

    /* Generate terms until we have at least n of them. */
    while (cache_len < (size_t)n) {
        /* Stop if the next candidate cannot be represented in seq_t. */
        if (next_candidate > (idx_t)MAX_SEQ_VAL) break;

        seq_t cand = (seq_t)next_candidate;
        seq_t s    = digit_sum(cand);    /* sum of decimal digits of cand */
        seq_t ph   = calc_phi(cand);     /* Euler's totient of cand */

        if (ph % s == 0) append_term(cand);

        ++next_candidate;
    }

    /* If the requested index was not produced (e.g. seq_t too small),
       return 0 as a safe fallback. */
    return (cache_len >= (size_t)n) ? cache[n-1] : 0;
}
