/*
 * A180591  G.f.: A(x) = exp( Sum_{n>=1} 2^[A001511(n)^2]*x^n/n ) where
 *          A001511(n) is the exponent in the highest power of 2 that divides
 *          2n.
 * https://oeis.org/A180591
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (1/n) * Σ_{k=1..n} 2^{(v2(k)+1)^2} * a(n-k), a(0)=1 where
 *            v2(k) is the exponent of 2 in the factorization of k (the number
 *            of trailing zeros in binary)
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 16, u64 n <= 24, u128 n <= 24,
 *            big any n
 * Validated: partial, pass, 25 terms through n = 24 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/mem.h"

/* Compute 2^exp for any exponent that may not fit in the bit width of seq_t. */
static seq_t pow2(idx_t exp)
{
    seq_t res = (seq_t)1;
    for (idx_t i = 0; i < exp; ++i)
        res *= (seq_t)2;
    return res;
}

/* Utility: exponent of 2 dividing n, assumes n>0 */
static inline idx_t trailing_zeros(idx_t n) {
    return (idx_t)__builtin_ctzll((unsigned long long)n);
}

/* Memoized computation of a(n) */
seq_t a(idx_t n) {
    /* Static caches */
    static seq_t *a_cache = NULL;
    static idx_t a_cap   = 0;
    static seq_t *b_cache = NULL;
    static idx_t b_cap   = 0;

    /* Initialise cache for a(0) */
    if (a_cap == 0) {
        a_cache = mem_new(seq_t, 1);
        if (!a_cache) return 0;         /* allocation failure */
        a_cache[0] = 1;
        a_cap = 1;
    }

    /* Create b cache on demand */
    if (b_cap == 0) {
        b_cache = mem_new(seq_t, 1);
        if (!b_cache) return 0;
        b_cap = 0;
    }

    if (n >= a_cap) {
        /* Extend b cache up to n */
        if (n >= b_cap) {
            seq_t *new_b = mem_resize(seq_t, b_cache, (n + 1));
            if (!new_b) return 0;          /* allocation failure */
            b_cache = new_b;
            for (idx_t i = b_cap; i <= n; ++i) {
                /* v2(i) = trailing zero bits of i */
                idx_t tz = trailing_zeros(i);
                idx_t exp = (tz + 1) * (tz + 1);  /* (v2(i)+1)^2 */
                b_cache[i] = pow2(exp);
            }
            b_cap = n + 1;
        }

        /* Extend a cache up to n */
        seq_t *new_a = mem_resize(seq_t, a_cache, (n + 1));
        if (!new_a) return 0;
        a_cache = new_a;
        for (idx_t i = a_cap; i <= n; ++i) {
            seq_t sum = 0;
            for (idx_t k = 1; k <= i; ++k) {
                sum = sum + b_cache[k] * a_cache[i - k];
            }
            a_cache[i] = sum / (seq_t)i;   /* exact division */
        }
        a_cap = n + 1;
    }

    return a_cache[n];
}
