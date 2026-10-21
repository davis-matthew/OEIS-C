/*
 * A213203  The sum of the first n! integers, with every n-th integer taken as
 *          negative.
 * https://oeis.org/A213203
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = n*(n-1)!*((n-2)*(n-1)! - 1)/2
 * Range:     i8 n <= 3, i16 n <= 5, i32 n <= 8, i64 n <= 12, i128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence used: a(1) = -1, a(2) = -1, a(3) = 3, a(4) = 132 */
/*  a(n) = (n^2+n+11)*a(n-1) - (n^3+7*n^2-13*n+39)*a(n-2) */
/*          + 2*(n-2)*(4*n^2-2*n-15)*a(n-3) - 20*(n-2)*(n-3)*(n-4)*a(n-4) */

#include <stdlib.h>
#include <stdint.h>

/* Static cache for computed terms */
static seq_t *cache = NULL;
static size_t cache_cap = 0;
static idx_t cache_max_computed = 0;

/* Ensure the cache can hold indices up to 'need' (inclusive). */
static void ensure_capacity(idx_t need)
{
    if ((idx_t)cache_cap >= need)
        return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 16;
    while (new_cap < (size_t)need)
        new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    cache = tmp;
    cache_cap = new_cap;
}

/* Initialise cache with the first four terms. */
static void init_cache(void)
{
    if (cache != NULL)
        return;
    ensure_capacity(5);
    cache[1] = (seq_t)(-1);
    cache[2] = (seq_t)(-1);
    cache[3] = (seq_t)3;
    cache[4] = (seq_t)132;
    cache_max_computed = 4;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;  /* offset is 1; undefined for n < 1 */

    init_cache();

    /* Compute terms up to n if necessary. */
    if ((idx_t)cache_cap <= n)
        ensure_capacity(n + 1);

    if (n <= cache_max_computed)
        return cache[n];

    /* Compute from cache_max_computed+1 up to n. */
    for (idx_t i = cache_max_computed + 1; i <= n; ++i)
    {
        seq_t nn  = (seq_t)i;
        seq_t n2  = nn * nn;                /* n^2 */
        seq_t n3  = n2 * nn;                /* n^3 */

        seq_t poly1 = n2 + nn + 11;                         /* n^2 + n + 11 */
        seq_t poly2 = n3 + 7 * n2 - 13 * nn + 39;           /* n^3 + 7n^2 -13n +39 */
        seq_t poly3 = 2 * ((nn - 2) * (4 * n2 - 2 * nn - 15));   /* 2*(n-2)*(4n^2-2n-15) */
        seq_t poly4 = 20 * (nn - 2) * (nn - 3) * (nn - 4);   /* 20*(n-2)*(n-3)*(n-4) */

        seq_t t1 = poly1 * cache[i - 1];
        seq_t t2 = poly2 * cache[i - 2];
        seq_t t3 = poly3 * cache[i - 3];
        seq_t t4 = poly4 * cache[i - 4];

        cache[i] = t1 - t2 + t3 - t4;
    }

    cache_max_computed = n;
    return cache[n];
}
