/*
 * A213170  E.g.f.: exp(2*(1-exp(x))).
 * https://oeis.org/A213170
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   not stated
 * Range:     i8 n <= 6, i16 n <= 11, i32 n <= 17, i64 n <= 27, i128 n <= 45,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A213170
 * Formula:  a(n) = B_n(-2)          (Bell polynomial at –2)
 * Recurrence: a(0) = 1
 *              a(n) = –2 * Σ_{k=0}^{n-1} binom(n-1,k) * a(k)   for n ≥ 1
 * This implementation memoises results in a static array.
 * Calls may be in any order; new values are computed lazily.
 */

#include <stdlib.h>   /* malloc, realloc, free */

/* static cache for computed values */
static seq_t *cache = NULL;
static size_t cache_size = 0;      /* allocated number of elements */
static idx_t  computed_upto = 0;   /* next index that has not been computed yet */

/* ensure cache has room for indices 0..n (inclusive) */
static void
ensure_capacity(idx_t n)
{
    if (cache == NULL || cache_size <= (size_t)n) {
        size_t new_size = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (tmp == NULL)  /* allocation failed */
            return;        /* let the caller handle other errors */
        cache = tmp;
        cache_size = new_size;
    }
}

seq_t a(idx_t n)
{
    /* make sure we can address cache[n] */
    ensure_capacity(n);

    /* initialise a(0) on first use */
    if (computed_upto == 0) {
        cache[0] = (seq_t)1;
        computed_upto = 1;
    }

    /* compute any missing values up to n */
    for (idx_t i = computed_upto; i <= n; ++i) {
        /* compute sum_{k=0}^{i-1} binom(i-1,k) * a(k) */
        seq_t sum = 0;
        seq_t c   = (seq_t)1;   /* binomial coefficient C(i-1,0) */

        for (idx_t k = 0; k < i; ++k) {
            sum += c * cache[k];

            /* update c to C(i-1, k+1) if needed */
            if (k + 1 < i) {
                c = c * (seq_t)(i - 1 - k) / (seq_t)(k + 1);
            }
        }

        cache[i] = -((seq_t)2) * sum;
    }

    computed_upto = n + 1;   /* all values up to n are now computed */

    return cache[n];
}
