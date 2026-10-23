/*
 * A122593  a(n) = -a(n-1) + a(n-3) - (a(n-1) - a(n-2))^2 + (a(n-2) -
 *          a(n-3))^2.
 * https://oeis.org/A122593
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = -a(n-1) + a(n-3) - (a(n-1) - a(n-2))^2 + (a(n-2) -
 *            a(n-3))^2, with a(1) = a(2) = a(3) = 1
 * Range:     i8 n <= 8, i16 n <= 9, i32 n <= 10, i64 n <= 11, i128 n <= 12,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *seq_cache = NULL;   /* array of computed terms, index 1..n */
static size_t cache_alloc = 0;    /* number of allocated elements */
static idx_t cache_max = 0;       /* largest index that has a value stored */

/* Return the n-th term of OEIS A122593 */
seq_t a(idx_t n)
{
    /* Ensure the first three terms are available */
    if (cache_max < 3)
    {
        size_t needed = 4;                 /* indices 0..3, index 0 unused */
        if (cache_alloc < needed)
        {
            seq_t *tmp = (seq_t *)realloc(seq_cache, needed * sizeof(seq_t));
            seq_cache = tmp;
            cache_alloc = needed;
        }
        seq_cache[1] = seq_cache[2] = seq_cache[3] = (seq_t)1;
        cache_max = 3;
    }

    /* Expand the cache if necessary */
    if ((size_t)n + 1 > cache_alloc)
    {
        size_t newsize = (size_t)n + 1;
        if (newsize < 16) newsize = 16;
        if (newsize < cache_alloc * 2) newsize = cache_alloc * 2;
        seq_t *tmp = (seq_t *)realloc(seq_cache, newsize * sizeof(seq_t));
        seq_cache = tmp;
        cache_alloc = newsize;
    }

    /* Compute any missing terms up to n */
    for (idx_t i = cache_max + 1; i <= n; ++i)
    {
        seq_t a1 = seq_cache[i - 1];
        seq_t a2 = seq_cache[i - 2];
        seq_t a3 = seq_cache[i - 3];

        seq_t diff1 = a1 - a2;    /* a(i-1) - a(i-2) */
        seq_t diff2 = a2 - a3;    /* a(i-2) - a(i-3) */

        seq_t term = -a1 + a3 - (diff1 * diff1) + (diff2 * diff2);
        seq_cache[i] = term;
    }
    cache_max = n;

    return seq_cache[n];
}
