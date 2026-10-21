/*
 * A220622  Number of ways to reciprocally link elements of an 3 X n array
 *          either to themselves or to exactly one horizontal, diagonal and
 *          antidiagonal neighbor.
 * https://oeis.org/A220622
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 6*a(n-1) + 22*a(n-2) - 32*a(n-3) - 12*a(n-4) + 8*a(n-5) -
 *            3*a(n-6) with initial values: a(1)=1, a(2)=22, a(3)=125,
 *            a(4)=1189, a(5)=9176, a(6)=76955
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 11, u64 n <= 21, u128 n <= 42,
 *            big any n
 * Helpers:   seq_alloc_new_array, seq_free_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>                       /* for size_t */
#include "../utils/seq_alloc_new_array.h" /* seq_alloc_new_array */
#include "../utils/seq_free_array.h"      /* seq_free_array */

/*------------------------------------------------------------------*/
/*  Cache for memoized values                                        */
/*------------------------------------------------------------------*/
static seq_t *cache = NULL;             /* array of computed terms      */
static idx_t  cache_len = 0;            /* number of terms currently stored */

/*------------------------------------------------------------------*/
/*  Grow the cache up to size n and compute any missing terms       */
/*------------------------------------------------------------------*/
static void ensure_cache(idx_t n)
{
    if (n <= cache_len)
        return;               /* nothing to do */

    /* Allocate new bigger buffer */
    seq_t *new_cache = seq_alloc_new_array((size_t)n);
    if (!new_cache) {
        /* Allocation failure – terminate the program */
        abort();
    }

    /* Copy previously computed terms */
    for (idx_t i = 0; i < cache_len; ++i)
        new_cache[i] = cache[i];

    /* Free old buffer, if any */
    if (cache)
        seq_free_array(cache);

    cache = new_cache;

    /* Compute new terms from cache_len+1 to n */
    for (idx_t i = cache_len + 1; i <= n; ++i) {
        if (i == 1)
            cache[0] = (seq_t)1;
        else if (i == 2)
            cache[1] = (seq_t)22;
        else if (i == 3)
            cache[2] = (seq_t)125;
        else if (i == 4)
            cache[3] = (seq_t)1189;
        else if (i == 5)
            cache[4] = (seq_t)9176;
        else if (i == 6)
            cache[5] = (seq_t)76955;
        else {
            /* a(i) = 6*a(i-1) + 22*a(i-2) - 32*a(i-3) - 12*a(i-4) + 8*a(i-5) - 3*a(i-6) */
            seq_t a1 = cache[i - 2]; /* a(i-1) */
            seq_t a2 = cache[i - 3]; /* a(i-2) */
            seq_t a3 = cache[i - 4]; /* a(i-3) */
            seq_t a4 = cache[i - 5]; /* a(i-4) */
            seq_t a5 = cache[i - 6]; /* a(i-5) */
            seq_t a6 = cache[i - 7]; /* a(i-6) */

            cache[i - 1] = a1 * (seq_t)6
                         + a2 * (seq_t)22
                         - a3 * (seq_t)32
                         - a4 * (seq_t)12
                         + a5 * (seq_t)8
                         - a6 * (seq_t)3;
        }
    }

    cache_len = n;
}

/*------------------------------------------------------------------*/
/*  Public entry point: returns a(n) for n >= 1                     */
/*------------------------------------------------------------------*/
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;          /* undefined for n < 1 – behaviour irrelevant */

    ensure_cache(n);
    return cache[n - 1];
}
