/*
 * A162415  L.g.f.: Sum_{n>=1} a(n)*x^n/n = log( Sum_{n>=0} x^(2^n-1) ).
 * https://oeis.org/A162415
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = sum_{k>=1, 2^k <= n+1} (2^k-1) * b[n-(2^k-1)], where the
 *            sequence b satisfies (1 + x + x^3 + x^7 + …) * b(x) = 1 (i.e. b
 *            = 1 / (1 + x + x^3 + x^7 + …) )
 * Range:     i8 n <= 11, i16 n <= 24, i32 n <= 51, i64 n <= 104,
 *            i128 n <= 210, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* Forward declaration of the main function */
seq_t a(idx_t n);

/* Static cache of the reciprocal sequence b.
   b[i] corresponds to the coefficient of x^i in 1/(1+x+x^3+x^7+…). */
static seq_t *b_cache = NULL;
static idx_t b_cache_len = 0;           /* highest index computed, inclusive */

/* Ensure b_cache contains coefficients up to max_n (inclusive). */
static void
ensure_b_cache(idx_t max_n)
{
    /* allocate if needed */
    if (b_cache == NULL) {
        b_cache = (seq_t *)malloc(sizeof(seq_t));
        if (!b_cache) abort();
        b_cache[0] = (seq_t)1;       /* constant term of B(x) */
        b_cache_len = 0;
    }

    if (max_n <= b_cache_len)
        return;

    /* Resize the array */
    seq_t *tmp = (seq_t *)realloc(b_cache, (max_n + 1) * sizeof(seq_t));
    if (!tmp) abort();
    b_cache = tmp;

    /* Compute new coefficients */
    for (idx_t i = b_cache_len + 1; i <= max_n; ++i) {
        seq_t sum = 0;
        /* sum over all powers 2^k-1 that are <= i */
        for (idx_t k = 1; ; ++k) {
            idx_t pow = ((idx_t)1 << k) - 1;
            if (pow > i)
                break;
            sum += b_cache[i - pow];
        }
        b_cache[i] = -sum;
    }

    b_cache_len = max_n;
}

/* Main sequence function */
seq_t
a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;                      /* outside the OEIS offset */

    /* We need b up to index n, because the largest index referenced is
       n - (2^k - 1) with 2^k <= n+1, hence 2^k-1 <= n. */
    ensure_b_cache(n);

    seq_t res = 0;
    /* Sum over all powers 2^k such that 2^k <= n+1. */
    for (idx_t k = 1; ; ++k) {
        idx_t pow2 = ((idx_t)1 << k);
        if (pow2 > n + 1)
            break;
        idx_t pow = pow2 - 1;                 /* weight = 2^k - 1 */
        idx_t idx = n - pow;                  /* coefficient of B at this index */
        res += (seq_t)pow * b_cache[idx];
    }
    return res;
}
