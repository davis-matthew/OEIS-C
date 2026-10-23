/*
 * A014619  Exponential generating function is -f(x) * Integral_{t = 0..x}
 *          exp(exp(-t) - 1) dt, where f(x) = exp(1 - x - exp(-x)) is the
 *          exponential generating function for A014182.
 * https://oeis.org/A014619
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   not stated
 * Range:     i8 n <= 7, i16 n <= 11, i32 n <= 18, i64 n <= 26, i128 n <= 26,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A014619
   b(0) = -1
   b(n) = -∑_{i=1..n} b(i‑1)*C(n,i)   (n≥1)
   a(n) = (-1)^n * b(n)                (n≥1)
   Computes values with unlimited precision using the provided
   binary‑coefficient routine and stores a running cache for speed. */

#include <stdlib.h>
#include "../utils/binom.h"

seq_t a(idx_t n)
{
    /* Cache of b-values, indexed from 0 upwards.
       b_cache[i] holds b(i), i ≤ b_cap - 1. */
    static seq_t *b_cache = NULL;
    static idx_t  b_cap   = 0;   /* number of elements actually stored      */
    static idx_t  b_alloc = 0;   /* allocated size of b_cache               */

    /* Initialisation: b(0) = -1 */
    if (b_cache == NULL) {
        b_cache   = (seq_t *)malloc(sizeof(seq_t));
        if (!b_cache) return (seq_t)0;          /* allocation failure */
        b_cache[0] = (seq_t)-1;
        b_cap     = 1;
        b_alloc   = 1;
    }

    /* Grow the cache (doubling strategy) until it contains all b(0)…b(n) */
    while (b_cap <= n) {
        /* Ensure sufficient storage for the new element */
        if (b_cap == b_alloc) {
            idx_t new_alloc = b_alloc * 2u;
            seq_t *new_ptr = (seq_t *)realloc(b_cache, new_alloc * sizeof(seq_t));
            if (!new_ptr) return (seq_t)0;      /* allocation failure */
            b_cache = new_ptr;
            b_alloc = new_alloc;
        }

        idx_t m = b_cap;      /* index of b to compute */
        seq_t sum = (seq_t)0;

        for (idx_t i = 1; i <= m; ++i) {
            seq_t bi1  = b_cache[i-1];
            seq_t comb = binom(m, i);          /* C(m,i) */
            sum += bi1 * comb;
        }
        b_cache[m] = -sum;          /* b(m) = -sum */
        ++b_cap;                    /* next empty slot */
    }

    /* a(n) = (-1)^n * b(n) */
    return ( (n & 1u) == 0 ) ?  b_cache[n] : -(b_cache[n]);
}
