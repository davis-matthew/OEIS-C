/*
 * A115625  Number of partitions of {1,...,n} into block sizes a power of 2.
 * https://oeis.org/A115625
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 10, u32 n <= 16, u64 n <= 27, u128 n <= 46,
 *            big any n
 * Validated: standard, pass, 42 terms through n = 515 (2026-11-01)
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A115625: partitions of {1,…,n} into blocks of size a power of 2.
   Recurrence: a(0)=1, a(n)=∑_{p=1,2,4,…≤n} C(n-1,p-1)·a(n-p)   (n≥1)
   where C are binomial coefficients. */

#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include "../utils/binomial.h"

/* Helper: safe multiplication for seq_t */
static inline seq_t seq_mul(seq_t a, seq_t b) {
    return a * b;
}

static inline seq_t seq_one(void) { return (seq_t)1; }

static seq_t* seq_realloc(seq_t* old, size_t old_cap, size_t new_cap) {
#ifdef __cplusplus
    seq_t* new_ptr = new seq_t[new_cap];
    for (size_t i = 0; i < old_cap; ++i)
        new_ptr[i] = old[i];
    delete[] old;
    return new_ptr;
#else
    return (seq_t*)realloc(old, new_cap * sizeof(seq_t));
#endif
}

static void _ensure_cache(idx_t n, seq_t **cache, size_t *cap)
{
    if (*cap <= n) {
        size_t newcap = (*cap == 0) ? 1 : (*cap << 1);
        while (newcap <= n) newcap <<= 1;
        seq_t *tmp = seq_realloc(*cache, *cap, newcap);
        if (!tmp) exit(1);                     /* out of memory */
        *cache = tmp;
        *cap   = newcap;
    }
}

seq_t a(idx_t n)
{
    static seq_t *cache   = NULL;
    static size_t cap     = 0;               /* allocated array size   */
    static idx_t max_computed = 0;          /* largest n already built */

    /* Ensure storage for all indices up to n */
    _ensure_cache(n, &cache, &cap);

    /* Initialise a(0) on first use */
    if (max_computed == 0)
        cache[0] = seq_one();

    /* If we already have the requested value, return it */
    if (n <= max_computed)
        return cache[n];

    /* Compute new values up to n */
    for (idx_t k = max_computed + 1; k <= n; ++k) {
        seq_t sum = 0;                       /* a(k) starts at 0      */
        for (idx_t p = 1; p <= k; p <<= 1) {
            seq_t coeff = binomial(k - 1, p - 1);            /* C(k-1,p-1)  */
            seq_t term  = seq_mul(coeff, cache[k - p]); /* coeff * a(k-p) */
            sum += term;
        }
        cache[k] = sum;
    }

    max_computed = n;
    return cache[n];
}
