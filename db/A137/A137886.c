/*
 * A137886  Number of (directed) Hamiltonian paths in the n-crown graph.
 * https://oeis.org/A137886
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 5, u32 n <= 8, u64 n <= 13, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A137886: Number of directed Hamiltonian paths in the n‑crown graph.
   Recurrence (n≥6): 
     a(n) = (n‑1)*n*a(n‑1) + (n‑1)²*n*a(n‑2) + (n‑2)*(n‑1)*n*a(n‑3)
   Initial values:
     a(3) = 12, a(4) = 144, a(5) = 3840
   Offset: 3
   The implementation memoises previously computed values so that calls
   are fast and independent of order.  All arithmetic is performed in
   the user‑supplied type seq_t; no intermediate values exceed what
   fits in seq_t.  No external libraries are required. */

#include <stdlib.h>

/* Memoisation cache: cache[i] holds a(i) for 0 ≤ i < cache_cap.
   cache_max records the highest index that has been computed. */
static seq_t *cache = NULL;
static idx_t cache_cap = 0;
static idx_t cache_max = 2;   /* nothing computed yet (only a(3)…a(5) are needed) */

/* Ensure the cache can hold indices up to n.  Allocate at least 6 slots to
   accommodate the three base cases a(3), a(4), a(5). */
static void ensure_cache(idx_t n)
{
    if (n < 3)
        return;

    idx_t needed = n + 1;
    if (needed < 6)
        needed = 6;
    if (needed <= cache_cap)
        return;

    idx_t old_cap = cache_cap;
    cache = (seq_t *)realloc(cache, needed * sizeof(seq_t));
    if (!cache)
        exit(1);  /* allocation failed */

    /* Initialise any newly added slots to zero. */
    for (idx_t i = old_cap; i < needed; ++i)
        cache[i] = (seq_t)0;

    cache_cap = needed;
}

/* Compute a(n) for n ≥ 3, memoised.  Values for n < 3 are defined as 0. */
seq_t a(idx_t n)
{
    if (n < 3)
        return (seq_t)0;

    ensure_cache(n);

    /* Initialise the first three known terms once. */
    if (cache_max < 5) {
        cache[3] = (seq_t)12;
        cache[4] = (seq_t)144;
        cache[5] = (seq_t)3840;
        cache_max = 5;
    }

    /* If already computed, return immediately. */
    if (cache[n] != (seq_t)0)
        return cache[n];

    /* Compute successive terms up to n. */
    for (idx_t i = cache_max + 1; i <= n; ++i) {
        /* Convert indices to seq_t for arithmetic. */
        seq_t i_seq      = (seq_t)i;
        seq_t i_minus1   = (seq_t)(i - 1);
        seq_t i_minus2   = (seq_t)(i - 2);

        /* (n-1)*n*a(n-1) */
        seq_t term1 = i_minus1 * i_seq * cache[i - 1];

        /* (n-1)^2*n*a(n-2) */
        seq_t term2 = i_minus1 * i_minus1 * i_seq * cache[i - 2];

        /* (n-2)*(n-1)*n*a(n-3) */
        seq_t term3 = i_minus2 * i_minus1 * i_seq * cache[i - 3];

        cache[i] = term1 + term2 + term3;
    }

    cache_max = n;
    return cache[n];
}
