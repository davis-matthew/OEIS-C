/*
 * A012019  E.g.f.: exp(sin(arctan(x))).
 * https://oeis.org/A012019
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(0)=1, a(1)=1, a(2)=1, a(3)=-2, a(4)=-11, a(5)=16, a(n) =
 *            -(3*n^2 -12*n +11)*a(n-2) -3*(n-4)*(n-3)^2*(n-2)*a(n-4)
 *            -(n-6)*(n-5)*(n-4)^2*(n-3)*(n-2)*a(n-6) for n≥6
 * Range:     i8 n <= 5, i16 n <= 9, i32 n <= 13, i64 n <= 20, i128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;   /* cache[0..current] */
static size_t cache_size = 0;

/* Ensure that cache has room for indices up to n */
static void ensure_cache(idx_t n)
{
    if (n < cache_size)
        return;
    size_t new_size = cache_size == 0 ? 8 : cache_size << 1;
    while (new_size <= n)
        new_size <<= 1;
    cache = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
    cache_size = new_size;
}

/* Compute the n‑th term of A012019 */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)1;
    if (n == 3) return (seq_t)(-2);
    if (n == 4) return (seq_t)(-11);
    if (n == 5) return (seq_t)16;

    ensure_cache(n);

    /* initialise known values if not already in cache */
    if (cache[0] == 0) cache[0] = (seq_t)1;
    if (cache[1] == 0) cache[1] = (seq_t)1;
    if (cache[2] == 0) cache[2] = (seq_t)1;
    if (cache[3] == 0) cache[3] = (seq_t)(-2);
    if (cache[4] == 0) cache[4] = (seq_t)(-11);
    if (cache[5] == 0) cache[5] = (seq_t)16;

    /* compute terms up to n */
    for (idx_t i = 6; i <= n; ++i)
    {
        seq_t term = 0;

        /* coefficient for a(i-2) */
        seq_t coeff1 = (seq_t)( (seq_t)3 * (seq_t)i * (seq_t)i
                               - (seq_t)12 * (seq_t)i
                               + (seq_t)11 );
        term -= coeff1 * cache[i - 2];

        /* coefficient for a(i-4) */
        seq_t coeff2 = (seq_t)3
                      * (seq_t)(i - 4)
                      * (seq_t)(i - 3)
                      * (seq_t)(i - 3)
                      * (seq_t)(i - 2);
        term -= coeff2 * cache[i - 4];

        /* coefficient for a(i-6) */
        seq_t coeff3 = (seq_t)(i - 6)
                      * (seq_t)(i - 5)
                      * (seq_t)(i - 4)
                      * (seq_t)(i - 4)
                      * (seq_t)(i - 3)
                      * (seq_t)(i - 2);
        term -= coeff3 * cache[i - 6];

        cache[i] = term;
    }

    return cache[n];
}
