/*
 * A170471  Number of reduced words of length n in Coxeter group on 30
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^45 = I.
 * https://oeis.org/A170471
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 28·∑_{k=1}^{44} a(n−k) – 406·a(n−45) for n ≥ 45. Initial
 *            values for 0 ≤ n < 45 are obtained from the generating function
 *            P(t)/Q(t). The code computes the first 45 terms by convolving
 *            the power‑series of 1/Q(t) with P(t); all further values use the
 *            recurrence. The sequence is cached so that...
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 13, u128 n <= 15,
 *            big any n
 * Helpers:   seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/seq_realloc.h"

/* seq_t and idx_t are supplied by the build system. */

/* --- static cache --- */
static seq_t *cache      = NULL;   /* array of computed terms      */
static size_t cache_size = 0;      /* number of valid entries          */
static size_t cache_cap  = 0;      /* allocated capacity               */
static bool   initialized = false;

/* Compute the first 45 terms (indices 0..44).  The coefficients of P(t) are
   1, then 44 copies of 2, then 1.  The coefficients of 1/Q(t) up to
   degree 44 satisfy
          s[0] = 1,
          s[n] = 28·(s[0] + … + s[n−1])   for 1 ≤ n ≤ 44. */
static void init_sequence(void)
{
    /* Series for 1/Q(t) up to degree 44. */
    seq_t s[46] = {0};
    s[0] = 1;
    seq_t sum_s = s[0];
    for (idx_t n = 1; n <= 44; ++n) {
        s[n] = (seq_t)28 * sum_s;
        sum_s += s[n];
    }

    /* Allocate cache for the first 45 terms. */
    cache = seq_realloc(NULL, 0, 45);
    if (cache == NULL) abort();
    cache_cap = 45;

    /* Convolution of P(t) and the s[t] coefficients gives a[0..44]. */
    seq_t conv_sum = 0;   /* sum of s[0] … s[n−1] for the current n */
    for (idx_t n = 0; n < 45; ++n) {
        seq_t val = s[n];             /* P[0]·s[n] = 1·s[n] */
        if (n > 0) {
            conv_sum += s[n - 1];      /* add s[n−1] to running sum */
            val += (seq_t)2 * conv_sum;/* P[1..n] are 2 for n≤44   */
        }
        cache[n] = val;
    }

    cache_size = 45;
    initialized = true;
}

/* Sequence accessor. */
seq_t a(idx_t n)
{
    if (!initialized)
        init_sequence();

    if (n < cache_size)
        return cache[n];

    /* Grow cache to accommodate index n. */
    size_t new_cap = n + 1;
    seq_t *new_cache = seq_realloc(cache, cache_cap, new_cap);
    if (new_cache == NULL) abort();
    cache = new_cache;
    cache_cap = new_cap;

    /* Extend cache using the recurrence. */
    for (idx_t i = cache_size; i <= n; ++i) {
        seq_t val = 0;
        for (idx_t k = 1; k <= 44; ++k)
            val += cache[i - k] * (seq_t)28;
        val -= cache[i - 45] * (seq_t)406;
        cache[i] = val;
    }

    cache_size = new_cap;
    return cache[n];
}
