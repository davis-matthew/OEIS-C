/*
 * A223329  Rolling cube footprints: number of n X 6 0..7 arrays starting with
 *          0 where 0..7 label vertices of a cube and every array movement to
 *          a horizontal or antidiagonal neighbor moves along a corresponding
 *          cube edge.
 * https://oeis.org/A223329
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 282*a(n-1) – 3969*a(n-2) + 9604*a(n-3) for n > 3 a(1) =
 *            243, a(2) = 64827, a(3) = 17324685
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 3, u64 n <= 7, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>

/* dynamic cache of computed terms */
static seq_t *cache     = NULL;          /* array index starts at 1            */
static size_t cache_cap = 0;             /* allocated capacity                */
static idx_t  cache_len = 0;             /* highest n computed (>=0)           */

/* grow the cache so that it can hold terms up to (and including) 'needed' */
static void ensure_cache_size(idx_t needed)
{
    if (cache_cap >= (size_t)needed + 1) return;   /* +1 to store index 0          */
    size_t new_cap = cache_cap ? cache_cap : 8;
    while (new_cap < (size_t)needed + 1) new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    cache = tmp;
    cache_cap = new_cap;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* offset guard; a(0) unused   */

    /* make sure the array has room for index n */
    ensure_cache_size(n);

    /* initialize the first three values on first use */
    if (cache_len < 3)
    {
        if (cache_len < 1) cache[1] = (seq_t)243;
        if (cache_len < 2) cache[2] = (seq_t)64827;
        if (cache_len < 3) cache[3] = (seq_t)17324685;
        cache_len = 3;
    }

    /* compute any missing terms up to n */
    if (n > cache_len)
    {
        for (idx_t i = cache_len + 1; i <= n; ++i)
        {
            seq_t t1 = (seq_t)282  * cache[i - 1];
            seq_t t2 = (seq_t)3969 * cache[i - 2];
            seq_t t3 = (seq_t)9604 * cache[i - 3];
            cache[i] = t1 - t2 + t3;
        }
        cache_len = n;
    }

    return cache[n];
}
