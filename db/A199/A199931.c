/*
 * A199931  Trisection 1 of A199803.
 * https://oeis.org/A199931
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = -a(n-1) - 5*a(n-2) + a(n-3) - a(n-4), a(0) = -1, a(1) =
 *            2, a(2) = 4, a(3) = -15
 * Range:     i8 n <= 6, i16 n <= 12, i32 n <= 26, i64 n <= 53, i128 n <= 108,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

static seq_t *cache = NULL;     /* allocated array of sequence values          */
static size_t cache_cap = 0;    /* current size of the allocated array         */
static idx_t cache_last = (idx_t)-1;  /* highest index that has been computed      */

/* Make sure that cache is large enough to hold indices 0 … n
   and compute any missing terms up to n.                     */
static void
ensure_cache(idx_t n)
{
    if ((size_t)n + 1 > cache_cap) {          /* expand the array if needed */
        size_t new_cap = cache_cap ? cache_cap * 2 : 4;
        while ((size_t)n + 1 > new_cap)
            new_cap *= 2;
        seq_t *new_cache = (seq_t *)malloc(new_cap * sizeof(seq_t));
        if (!new_cache) return;               /* caller deals with OOM        */
        if (cache) {
            for (size_t i = 0; i < cache_cap; ++i)
                new_cache[i] = cache[i];
            free(cache);
        }
        cache = new_cache;
        cache_cap = new_cap;
    }

    /* Initialise the first four terms if this is the first call. */
    if (cache_last == (idx_t)-1) {
        cache[0] = (seq_t)-1;
        cache[1] = (seq_t) 2;
        cache[2] = (seq_t) 4;
        cache[3] = (seq_t)-15;
        cache_last = 3;
    }

    /* If n < 4 we are already done. */
    if (n <= 3) return;

    /* Compute missing terms from cache_last+1 up to n. */
    for (idx_t i = cache_last + 1; i <= n; ++i) {
        seq_t a_nm1 = cache[i - 1];
        seq_t a_nm2 = cache[i - 2];
        seq_t a_nm3 = cache[i - 3];
        seq_t a_nm4 = cache[i - 4];
        cache[i] = -a_nm1 - ((seq_t)5 * a_nm2) + a_nm3 - a_nm4;
    }
    cache_last = n;
}

seq_t
a(idx_t n)
{
    if (cache == NULL || n > cache_last)
        ensure_cache(n);
    return cache[(size_t)n];
}
