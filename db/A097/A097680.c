/*
 * A097680  E.g.f.: (1/(1-x^5))*exp( 5*sum_{i>=0} x^(5*i+1)/(5*i+1) ) for an
 *          order-5 linear recurrence with varying coefficients.
 * https://oeis.org/A097680
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 5*a(n-1) + (n!/(n-5)!)*a(n-5) for n >= 5, a(0)=1, a(1)=5,
 *            a(2)=25, a(3)=125, a(4)=625
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 11, u64 n <= 19, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* static cache of already computed terms */
static seq_t *cache = NULL;
static idx_t cache_size = 0;

/* Ensure that terms 0..n are computed and stored in cache */
static void compute_upto(idx_t n)
{
    if (n < cache_size)
        return;                       /* already known */

    /* grow the cache array */
    seq_t *tmp = (seq_t *)realloc(cache, (n + 1) * sizeof(seq_t));
    if (!tmp)
        exit(EXIT_FAILURE);           /* allocation failure */
    cache = tmp;

    /* initialise base cases if this is the first time */
    if (cache_size == 0) {
        cache[0] = 1;                 /* 5^0 */
        seq_t v = 1;
        for (idx_t i = 1; i < 5; ++i) {   /* 5^1 .. 5^4 */
            v *= (seq_t)5;
            cache[i] = v;
        }
    }

    /* compute remaining terms up to n */
    for (idx_t i = cache_size; i <= n; ++i) {
        if (i < 5)
            continue;                 /* already set */

        /* compute n*(n-1)*(n-2)*(n-3)*(n-4) in seq_t */
        seq_t prod = (seq_t)i;
        prod *= (seq_t)(i - 1);
        prod *= (seq_t)(i - 2);
        prod *= (seq_t)(i - 3);
        prod *= (seq_t)(i - 4);

        /* recurrence: a(n) = 5*a(n-1) + prod * a(n-5) */
        cache[i] = (seq_t)5 * cache[i - 1] + prod * cache[i - 5];
    }

    cache_size = n + 1;
}

/* required public wrapper */
seq_t a(idx_t n)
{
    if (!cache)              /* lazy initialisation */
        compute_upto(4);     /* base cases */
    compute_upto(n);
    return cache[n];
}
