/*
 * A013360  Expansion of e.g.f.:
 *          exp(sin(x)-arctan(x))=1+1/3!*x^3-23/5!*x^5+10/6!*x^6+719/7!*x^7...
 * https://oeis.org/A013360
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = Σ_{odd k ≤ n} a(n‑k) * C(n‑1, k‑1) * (-1)^{(k‑1)/2} * (1
 *            – (k‑1)!) with a(0)=1
 * Range:     i8 n <= 6, i16 n <= 8, i32 n <= 13, i64 n <= 20, i128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* ---- Cache of computed a(i) ---- */
    static seq_t *cache   = NULL;   /* holds a(0)…a(max)   */
    static idx_t cache_cap = 0;     /* allocated size      */
    static idx_t cache_last = 0;    /* next index to compute */

    /* ---- Factorial table: fact[i] = i! for i < fact_cap ---- */
    static seq_t *fact   = NULL;
    static idx_t fact_cap = 0;      /* allocated size (≥ number of computed factorials) */

    /* 1. Ensure cache has enough capacity for indices up to n */
    if (n + 1 > cache_cap) {
        idx_t new_cap = n + 1;
        cache = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        cache_cap = new_cap;
    }

    /* 2. Initialise a(0) on first use */
    if (cache_last == 0) {
        cache[0] = (seq_t)1;
        cache_last = 1;   /* next index to compute */
    }

    /* 3. Ensure factorial table contains fact[0]…fact[n‑1] */
    idx_t needed_fact = (idx_t)n;          /* need indices up to n‑1 (or 0 if n==0) */
    if (needed_fact < 1) needed_fact = 1;  /* at least fact[0] */
    if (needed_fact > fact_cap) {
        idx_t old_cap = fact_cap;
        idx_t new_cap = needed_fact;
        fact = (seq_t*)realloc(fact, new_cap * sizeof(seq_t));

        /* Initialise new entries */
        if (old_cap == 0) {
            fact[0] = (seq_t)1;                /* 0! = 1 */
            old_cap = 1;
        }
        for (idx_t i = old_cap; i < new_cap; ++i) {
            fact[i] = fact[i - 1] * (seq_t)i;
        }
        fact_cap = new_cap;
    }

    /* 4. Compute any missing a(i) up to n */
    for (idx_t i = cache_last; i <= n; ++i) {
        if (i == 0) continue;   /* a(0) already set */
        seq_t sum = (seq_t)0;

        /* iterate over odd k ≤ i */
        for (idx_t k = 1; k <= i; k += 2) {
            idx_t m = (k - 1) / 2;   /* exponent for the sign */

            /* binomial coefficient C(i-1, k-1) using factorials */
            seq_t bin = fact[i - 1] / fact[k - 1];
            bin = bin / fact[i - k];

            /* (k-1)! */
            seq_t d = fact[k - 1];

            /* factor = (-1)^m * (1 - d) */
            seq_t factor;
            if ((m & 1u) == 0) {
                /* (-1)^m = +1 */
                factor = (seq_t)1 - d;
            } else {
                /* (-1)^m = -1 */
                factor = d - (seq_t)1;
            }

            /* term = a(i-k) * bin * factor */
            seq_t term = cache[i - k];
            term *= bin;
            term *= factor;

            sum += term;
        }
        cache[i] = sum;
    }
    cache_last = n + 1;   /* all indices up to n are now computed */
    return cache[n];
}
