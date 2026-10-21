/*
 * A052324  Number of increasing rooted trees with a forbidden limb of length
 *          3.
 * https://oeis.org/A052324
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, eigen
 * Formula:   a(n) is defined by the ordinary generating function A'(x) =
 *            exp(A(x) - x^3/6), with A(0)=0. From this differential equation
 *            one obtains the recurrence a(0) = 0, a(1) = 1, a(2) = 1, and for
 *            n ≥ 2 a(n+1) = Σ_{k=1}^{n} C(n-1,k-1)·a(k)·a(n-k+1) -
 *            ((n-1)(n-2)/2)·a(n-2)
 * Range:     u8 n <= 6, u16 n <= 9, u32 n <= 14, u64 n <= 22, u128 n <= 35,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Note:  seq_t is an arbitrary-precision integer type (or a
   built‑in unsigned integer type).  idx_t is an unsigned 64‑bit
   integer.  All arithmetic is performed using the operators of
   seq_t; the library provides a fast binomial function. */

#include <stdlib.h>
#include "../utils/binom.h"   /* for binom(idx_t a, idx_t b) */

seq_t a(idx_t n) {
    /* Simple memoisation – only grow the cache,
       never shrink it.  Cache entries are valid
       for all indices that have been computed. */
    static idx_t      cache_len   = 0;   /* length of allocated array   */
    static size_t     cache_cap   = 0;   /* number of elements available */
    static seq_t     *cache       = NULL;/* array of seq_t values       */

    /* Allocate/reallocate cache if necessary. */
    if (cache_cap <= n) {
        size_t new_cap = n + 1;
        void *ptr = realloc(cache, new_cap * sizeof(seq_t));
        if (!ptr) { free(cache); exit(1); }   /* simple error handling */
        cache = (seq_t *)ptr;

        /* Initialise new elements to zero. */
        for (idx_t i = cache_cap; i < new_cap; ++i)
            cache[i] = 0;
        cache_cap = new_cap;
    }

    /* Return cached value if already computed. */
    if (cache_len > n)
        return cache[n];

    /* Compute sequentially from the first missing index. */
    for (idx_t i = cache_len; i <= n; ++i) {
        if (i == 0) {          /* a(0) = 0  (already zero)            */
            cache[i] = 0;
            continue;
        }

        if (i == 1) {          /* a(1) = 1                            */
            cache[i] = 1;
            continue;
        }

        if (i == 2) {          /* a(2) = 1                            */
            cache[i] = 1;
            continue;
        }

        /* For i ≥ 3, use the recurrence.  Here n = i-1. */
        idx_t n_ = i - 1;
        seq_t sum = 0;

        /* Inner sum:  Σ_{k=1}^{n} C(n-1,k-1)·a(k)·a(n-k+1) */
        for (idx_t k = 1; k <= n_; ++k) {
            seq_t comb = binom(n_-1, k-1);        /* C(n-1, k-1)              */
            seq_t term = comb * cache[k] * cache[n_-k+1];
            sum = sum + term;
        }

        /* Subtraction of the forbidden-limb contribution:
           ((n-1)(n-2)/2)·a(n-2)      */
        seq_t coeff = ((seq_t)(n_-1) * (n_-2)) / 2;
        seq_t sub    = coeff * cache[n_-2];

        cache[i] = sum - sub;
    }

    cache_len = n + 1;
    return cache[n];
}
