/*
 * A115626  Number of non-squashing partitions of {1,...,n}.
 * https://oeis.org/A115626
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{i=0..⌈n/2⌉-1} binomial(n,i)*a(i) + (n even) * *
 *            binomial(n,n/2)*(a(n/2)-1/2). Initial value: a(0)=1
 * Range:     u8 n <= 7, u16 n <= 11, u32 n <= 20, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>
#include "../utils/binomial.h"   /* seq_t binomial(idx_t a, idx_t b) */

static seq_t *cache = NULL;   /* memoised values a(0)...a(size-1)   */
static size_t  cap   = 0;     /* allocated cache size               */
static size_t  size  = 0;     /* number of values already computed  */

/* Ensure the cache can hold indices 0..n.  Reallocate if necessary. */
static void ensure_cache(idx_t n)
{
    /* n is a 64‑bit unsigned integer; we need to compute n+1 safely. */
    size_t needed = (size_t)n + 1;
    if (needed <= cap) return;

    /* Allocate a new, larger array.  Double the capacity for amortised O(1). */
    size_t new_cap = (cap == 0 ? 1 : cap);
    while (new_cap < needed) new_cap <<= 1;

    seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!new_cache) abort();              /* out of memory – terminate */
    cache = new_cache;
    cap   = new_cap;
}

/* Main entry point.  Returns a(n) for any n >= 0. */
seq_t a(idx_t n)
{
    /* Initialise a(0)=1 if this is the first call. */
    if (size == 0) {
        ensure_cache(n);
        cache[0] = (seq_t)1;
        size = 1;
        if (n == 0) return cache[0];
    }

    /* If we already have the value, return immediately. */
    if (n < size) return cache[n];

    /* Compute all missing terms up to n (iteratively). */
    ensure_cache(n);
    for (idx_t k = size; k <= n; ++k) {
        if (k == 0) continue;   /* a(0) already set */

        seq_t sum = (seq_t)0;

        /* Number of terms in the first sum = ceil(k/2). */
        idx_t i_end = (k + 1) / 2;          /* i: 0 .. i_end-1 */

        /* Compute binomial(k,i) iteratively to avoid repeated calls. */
        seq_t bi = 1;                       /* binomial(k,0) */
        for (idx_t i = 0; i < i_end; ++i) {
            sum += bi * cache[i];
            /* Update bi to binomial(k,i+1) if needed. */
            if (i + 1 < i_end) {
                bi = bi * (seq_t)(k - i) / (seq_t)(i + 1);
            }
        }

        /* If k is even, add the extra term from the formula. */
        if ((k & 1) == 0) {
            seq_t bin_half = binomial(k, k/2);           /* binomial(k,k/2) */
            seq_t half_div = bin_half / (seq_t)2;        /* bin_half / 2   */
            sum += bin_half * cache[k/2] - half_div;
        }

        cache[k] = sum;
    }

    return cache[n];
}
