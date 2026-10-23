/*
 * A185654  G.f.: exp( Sum_{n>=1} -sigma(3n)*x^n/n ).
 * https://oeis.org/A185654
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   not stated
 * Range:     i8 n <= 15, i16 n <= 712, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula:  a(0) = 1
 *          a(n) = -(1/n) * Σ_{k=1..n} sigma(3k) * a(n-k)   (n≥1)
 *          where sigma(m) = sum of the positive divisors of m.
 * This implementation memoises values so calls can be in any order.
 */

#include <stdlib.h>

/* cache for computed terms of the sequence */
static seq_t *a_cache = NULL;
static size_t a_cap   = 0;        /* allocated capacity */
static idx_t a_len    = 0;        /* number of values already computed */

/* cache for divisor sums sigma(0..max) */
static seq_t *sigma_cache = NULL;
static size_t sigma_cap   = 0;
static idx_t sigma_len    = 0;    /* length of valid data in sigma_cache */

/* ensure a_cache can hold values up to idx (inclusive) */
static void ensure_a_capacity(idx_t idx)
{
    if ((size_t)idx + 1 <= a_cap)
        return;

    size_t new_cap = (size_t)idx + 1;
    seq_t *tmp = (seq_t *)realloc(a_cache, new_cap * sizeof(seq_t));
    if (!tmp) /* allocation failed – caller will treat as error */ return;

    a_cache = tmp;
    a_cap   = new_cap;
}

/* recompute sigma_cache up to new_max (inclusive, all indices) */
static void recompute_sigma(idx_t new_max)
{
    size_t new_cap = (size_t)new_max + 1;
    seq_t *tmp = (seq_t *)malloc(new_cap * sizeof(seq_t));
    if (!tmp) return;

    /* initialise all to 0 */
    for (size_t i = 0; i < new_cap; ++i)
        tmp[i] = (seq_t)0;

    /* classic divisor-sum sieve */
    for (idx_t d = 1; d <= new_max; ++d) {
        for (idx_t m = d; m <= new_max; m += d)
            tmp[m] += (seq_t)d;
    }

    if (sigma_cache) free(sigma_cache);
    sigma_cache = tmp;
    sigma_cap   = new_cap;
    sigma_len   = new_max + 1;
}

/* ensure sigma_cache contains values up to required_max (inclusive) */
static void ensure_sigma_cache(idx_t required_max)
{
    if ((size_t)required_max + 1 <= sigma_cap)
        return;

    recompute_sigma(required_max);
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;  /* a(0) = 1 */

    /* prepare caches */
    ensure_a_capacity(n);
    ensure_sigma_cache(3 * n);

    /* initialise a_cache[0] if not already done */
    if (a_len == 0) {
        a_cache[0] = (seq_t)1;
        a_len = 1;
    }

    /* compute missing terms */
    for (idx_t i = a_len; i <= n; ++i) {
        seq_t sum = (seq_t)0;
        for (idx_t k = 1; k <= i; ++k) {
            seq_t sigma_val = sigma_cache[3 * k];
            seq_t a_val     = a_cache[i - k];
            sum += sigma_val * a_val;
        }
        a_cache[i] = - (sum / (seq_t)i);
    }
    a_len = n + 1;

    return a_cache[n];
}
