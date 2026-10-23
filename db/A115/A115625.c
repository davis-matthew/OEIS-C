/*
 * A115625  Number of partitions of {1,...,n} into block sizes a power of 2.
 * https://oeis.org/A115625
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 10, u32 n <= 16, u64 n <= 27, u128 n <= 46,
 *            big any n
 * Helpers:   binom, seq_mul, seq_one, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A115625: partitions of {1,…,n} into blocks of size a power of 2.
   Recurrence: a(0)=1, a(n)=∑_{p=1,2,4,…≤n} C(n-1,p-1)·a(n-p)   (n≥1)
   where C are binomial coefficients. */

#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/binom.h"      /* seq_t binom(idx_t, idx_t)              */
#include "../utils/seq_mul.h"    /* seq_t seq_mul(seq_t, seq_t)           */
#include "../utils/seq_one.h"    /* seq_t seq_one(void)                   */
#include "../utils/seq_realloc.h"/* seq_t* seq_realloc(seq_t*,size_t,size_t) */

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
            seq_t coeff = binom(k - 1, p - 1);            /* C(k-1,p-1)  */
            seq_t term  = seq_mul(coeff, cache[k - p]); /* coeff * a(k-p) */
            sum += term;
        }
        cache[k] = sum;
    }

    max_computed = n;
    return cache[n];
}
