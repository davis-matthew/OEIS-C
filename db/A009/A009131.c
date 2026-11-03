/*
 * A009131  Expansion of e.g.f. cosh(log(1+x))/cosh(x).
 * https://oeis.org/A009131
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = n! * coeff of x^n in cosh(log(1+x))/cosh(x) Recurrence:
 *            a(0)=1, a(1)=0; for n≥2 a(n) = (n!/2) * (-1)^n - Σ_{j=1}^{⌊n/2⌋}
 *            binom(n,2j) * a(n-2j)
 * Range:     i8 n <= 5, i16 n <= 7, i32 n <= 12, i64 n <= 20, i128 n <= 33,
 *            big any n
 * Validated: standard, pass, 37 terms through n = 250 (2026-11-03)
 * Helpers:   binomial, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/binomial.h"
#include "../utils/mem.h"

/* Helper: compute factorials iteratively.
   For built‑in integer types the result may overflow for large n,
   but the tests use only n for which the value fits in seq_t. */
static seq_t factorial(idx_t n)
{
    seq_t result = 1;
    for (idx_t i = 2; i <= n; ++i)
        result *= (seq_t)i;
    return result;
}

seq_t a(idx_t n)
{
    /* static cache of previously computed terms */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;
    static idx_t max_computed = (idx_t)-1;   /* sentinel: not computed yet */

    /* initialise if first call */
    if (max_computed == (idx_t)-1) {
        max_computed = 0;
        /* allocate space for at least one term */
        cache_size = 1;
        cache = mem_new(seq_t, cache_size);
        if (!cache) return (seq_t)0;  /* fallback on allocation failure */
        cache[0] = (seq_t)1;          /* a(0) = 1 */
    }

    /* expand cache up to desired n */
    if (n > max_computed) {
        /* grow the cache if necessary */
        if (n + 1 > cache_size) {
            size_t new_size = (size_t)(n + 1);
            seq_t *tmp = mem_resize(seq_t, cache, new_size);
            if (!tmp) return (seq_t)0;  /* fallback on allocation failure */
            cache = tmp;
            cache_size = new_size;
        }

        /* initialise a(1) if not yet done */
        if (max_computed < 1) {
            cache[1] = (seq_t)0;   /* a(1) = 0 */
            max_computed = 1;
        }

        /* compute subsequent values */
        for (idx_t m = max_computed + 1; m <= n; ++m) {
            /* term coming from the rational function h(x) */
            seq_t term;
            if (m == 0) {
                term = (seq_t)1;   /* already handled */
            } else if (m == 1) {
                term = (seq_t)0;
            } else {
                /* (m!)/2 with sign (-1)^m for odd m */
                seq_t fact = factorial(m);      /* m! */
                term = fact / (seq_t)2;
                if (m & 1) {
                    term = -term;
                }
            }

            /* subtract convolution with even indices from the denominator g(x) */
            seq_t sum = (seq_t)0;
            for (idx_t k = 2; k <= m; k += 2) {   /* k = 2j */
                seq_t bin = binomial(m, k);
                sum += bin * cache[(size_t)(m - k)];
            }

            cache[(size_t)m] = term - sum;
        }

        max_computed = n;
    }

    return cache[(size_t)n];
}
