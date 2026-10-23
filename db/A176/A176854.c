/*
 * A176854  Sequence defined by the recurrence formula
 *          a(n+1)=sum(a(p)*a(n-p)+k,p=0..n)+l for n>=1, with here a(0)=1,
 *          a(1)=0, k=-1 and l=0.
 * https://oeis.org/A176854
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign
 * Formula:   not stated
 * Range:     i8 n <= 4, i16 n <= 9, i32 n <= 19, i64 n <= 26, i128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A176854
 * Recurrence:
 *   a(0) = 1, a(1) = 0, a(2) = -2, a(3) = -7
 *   for n ≥ 4
 *   (n+1)·a(n) + (-7n+2)·a(n-1) + (19n-29)·a(n-2)
 *               + (-17n+40)·a(n-3) + 2(2n-7)·a(n-4) = 0
 *   which gives
 *   a(n) = -{ (-7n+2)a(n-1) + (19n-29)a(n-2)
 *                + (-17n+40)a(n-3) + 2(2n-7)a(n-4) } / (n+1)
 * The recurrence is an O(n) direct algorithm.
 */

#include <stddef.h>
#include <stdlib.h>

/* Cached values of the sequence. */
static seq_t *cache = NULL;
static size_t cache_size = 0;     /* number of elements currently stored */
static size_t cache_cap  = 0;     /* capacity of the cache */

/* Initialize the cache with the first four values if necessary. */
static void ensure_cache_size(idx_t n)
{
    if (cache_size > n)
        return;

    /* Grow cache until it can hold index n */
    size_t new_cap = cache_cap ? cache_cap : 8;
    while (new_cap <= n)
        new_cap <<= 1;

    if (cache == NULL)
        cache = (seq_t *)malloc(new_cap * sizeof(seq_t));
    else
        cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));

    cache_cap = new_cap;

    /* Make sure the first four terms are present */
    if (cache_size == 0) {
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)0;
        cache[2] = (seq_t)(-2);
        cache[3] = (seq_t)(-7);
        cache_size = 4;
    } else {
        if (cache_size <= 1) cache[1] = (seq_t)0;
        if (cache_size <= 2) cache[2] = (seq_t)(-2);
        if (cache_size <= 3) cache[3] = (seq_t)(-7);
        cache_size = 4;
    }

    /* Compute terms up to n */
    for (idx_t i = cache_size; i <= n; ++i) {
        /* coefficients for the recurrence */
        long long c1 = -7 * (long long)i + 2;          /* -7n+2 */
        long long c2 = 19 * (long long)i - 29;         /* 19n-29 */
        long long c3 = -17 * (long long)i + 40;        /* -17n+40 */
        long long c4 = 2 * (2 * (long long)i - 7);     /* 2(2n-7) */

        seq_t term1 = (seq_t)c1 * cache[i - 1];
        seq_t term2 = (seq_t)c2 * cache[i - 2];
        seq_t term3 = (seq_t)c3 * cache[i - 3];
        seq_t term4 = (seq_t)c4 * cache[i - 4];

        seq_t sum       = term1 + term2 + term3 + term4;
        seq_t numerator = -sum;
        seq_t denom     = (seq_t)(i + 1);   /* n+1 */

        cache[i] = numerator / denom;      /* exact division */
    }

    cache_size = n + 1;
}

seq_t a(idx_t n)
{
    ensure_cache_size(n);
    return cache[n];
}
