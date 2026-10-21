/*
 * A066399  From reversion of e.g.f. for squares.
 * https://oeis.org/A066399
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n+1) = (-1)^n * n! * Σ_{m=0..n} (n+1)^m / m! * binomial(2n-m,
 *            n) a(0) = 0, a(1) = 1 For n ≥ 2 this is evaluated using the
 *            exact integer recurrence: term_0 = binomial(2n, n) * n!
 *            term_{m+1} = term_m * (n+1)/(m+1) * (n-m)/(2n-m) sum =
 *            Σ_{m=0..n} term_m a(n+1) = (-1)^n * sum
 * Range:     i8 n <= 3, i16 n <= 4, i32 n <= 8, i64 n <= 13, i128 n <= 22,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/binomial.h"

static seq_t *a_cache = NULL;
static seq_t *fact_cache = NULL;
static size_t cache_capacity = 0;
static size_t cache_size = 0; /* number of elements stored (0..cache_size-1) */

/* ensure cache holds at least 'needed' slots (indices 0..needed-1) */
static void grow_cache(size_t needed)
{
    if (needed <= cache_capacity)
        return;
    size_t new_cap = cache_capacity ? cache_capacity * 2 : 16;
    while (new_cap < needed)
        new_cap <<= 1;
    a_cache = (seq_t *)realloc(a_cache, new_cap * sizeof(seq_t));
    fact_cache = (seq_t *)realloc(fact_cache, new_cap * sizeof(seq_t));
    cache_capacity = new_cap;
}

seq_t a(idx_t n)
{
    /* base terms */
    if (n == 0) return 0;
    if (n == 1) return 1;

    /* ensure we have cached values up to n */
    if (cache_size <= (size_t)n) {
        grow_cache((size_t)n + 1);

        /* initialise missing entries */
        if (cache_size == 0) {
            a_cache[0] = 0;
            fact_cache[0] = 1;
            cache_size = 1;
        }
        if (cache_size <= 1) {
            a_cache[1] = 1;
            fact_cache[1] = 1;
            cache_size = 2;
        }

        /* compute all values up to index n */
        for (size_t i = cache_size; i <= (size_t)n; ++i) {
            idx_t k = (idx_t)i - 1;               /* k = i-1 (the 'n' in the formula) */

            /* term_0 = binomial(2k, k) * k! */
            seq_t term = binomial((idx_t)(2*k), (idx_t)k) * fact_cache[k];
            seq_t sum    = term;
            seq_t n_plus = (seq_t)(k + 1);            /* n+1 in the recurrence */

            for (idx_t m = 0; m < k; ++m) {
                term = term * n_plus;                /* * (k+1) */
                term = term / (m + 1);               /* / (m+1) */
                term = term * ((seq_t)(k - m));      /* * (k-m) */
                term = term / ((seq_t)(2*k - m));    /* / (2k-m) */
                sum += term;
            }

            seq_t a_val = sum;
            if ((k & 1) != 0)                       /* multiply by (-1)^k */
                a_val = -a_val;

            a_cache[i] = a_val;

            /* factorial for i to be used by future terms */
            fact_cache[i] = fact_cache[i - 1] * (seq_t)i;
        }
        cache_size = (size_t)n + 1;
    }

    return a_cache[n];
}
