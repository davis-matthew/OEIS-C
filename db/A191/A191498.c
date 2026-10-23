/*
 * A191498  E.g.f. sqrt(1+2*x*exp(2*x)).
 * https://oeis.org/A191498
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   not stated
 * Range:     i8 n <= 5, i16 n <= 8, i32 n <= 12, i64 n <= 19, i128 n <= 21,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence:  a(0) = 1;
   For n ≥ 1, a(n) = (2^n * n - Σ_{k=1}^{n-1} binom(n,k) * a(k) * a(n-k)) / 2
   derived from (A(x))^2 = 1 + 2x e^{2x}  where A(x) = sqrt(1+2x e^{2x}). */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/pow2.h"

seq_t a(idx_t n) {
    /* static cache for memoization */
    static seq_t *cache = NULL;
    static idx_t cached = 0;

    /* Initialise cache for a(0) if needed */
    if (!cache) {
        cache = (seq_t*)malloc(sizeof(seq_t));
        if (!cache) return (seq_t)0;      /* out of memory: return 0 as a safe fallback */
        cache[0] = (seq_t)1;
        cached = 0;
    }

    /* Extend cache up to n if necessary */
    if (n > cached) {
        cache = (seq_t*)realloc(cache, (size_t)(n+1) * sizeof(seq_t));
        if (!cache) return (seq_t)0;      /* out of memory */

        /* compute new values sequentially */
        for (idx_t i = cached + 1; i <= n; ++i) {
            if (i == 0) continue;          /* already set */

            seq_t term = 0;
            seq_t bin   = (seq_t)i;         /* binom(i,1) */

            for (idx_t k = 1; k < i; ++k) {
                seq_t prod = bin * cache[k] * cache[i - k];
                term += prod;
                /* update binom(i,k+1) = binom(i,k) * (i-k) / (k+1) */
                bin = bin * (seq_t)(i - k) / (seq_t)(k + 1);
            }

            seq_t rhs  = pow2(i) * (seq_t)i;
            seq_t diff = rhs - term;
            cache[i] = diff / 2; /* division is exact */
        }
        cached = n;
    }

    return cache[n];
}
