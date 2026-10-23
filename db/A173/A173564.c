/*
 * A173564  a(n) = a(n-1) + a(n-2) - [a(n-3)/4] - [a(n-4)/2] - [a(n-5)/4].
 * https://oeis.org/A173564
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) =
 *            a(n-1)+a(n-2)-Floor[a(n-3)/4]-Floor[a(n-4)/2]-Floor[a(n-5)/4],
 *            with a(0)=1, a(1)=1, a(2)=2, a(3)=3, a(4)=5
 * Range:     u8 n <= 15, u16 n <= 35, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   seq_alloc_array, seq_free_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>   /* for size_t */
#include "../utils/seq_alloc_array.h"        /* allocates an array of seq_t */
#include "../utils/seq_free_array.h"          /* frees an array of seq_t */

/* static storage for the cached sequence */
static seq_t   *cache = NULL;   /* array of computed values          */
static size_t   cap   = 0;      /* capacity of the array             */
static size_t   len   = 0;      /* number of values currently stored */

static void
ensure_capacity(size_t needed)
{
    if (cap >= needed)
        return;                       /* already enough space             */

    /* grow the array (at least twice, or to needed) */
    size_t new_cap = cap ? cap * 2 : needed;
    if (new_cap < needed)
        new_cap = needed;

    /* allocate new array – objects are constructed by seq_alloc_array */
    seq_t *tmp = seq_alloc_array(new_cap);
    /* copy existing values */
    for (size_t i = 0; i < len; ++i)
        tmp[i] = cache[i];
    /* free old array – construction/ destruction handled by seq_free_array */
    if (cache)
        seq_free_array(cache);

    cache = tmp;
    cap   = new_cap;
}

seq_t
a(idx_t n)
{
    /* prepare cache for indices 0 … n */
    ensure_capacity((size_t)n + 1);

    /* initialise the first five terms if needed (overwrite is harmless) */
    if (n >= 0) cache[0] = 1;
    if (n >= 1) cache[1] = 1;
    if (n >= 2) cache[2] = 2;
    if (n >= 3) cache[3] = 3;
    if (n >= 4) cache[4] = 5;

    /* compute further terms lazily */
    size_t start = len > 5 ? len : 5;   /* start computing past what is known   */
    for (size_t i = start; i <= (size_t)n; ++i) {
        seq_t v = cache[i-1] + cache[i-2];
        v -= cache[i-3] / 4;
        v -= cache[i-4] / 2;
        v -= cache[i-5] / 4;
        cache[i] = v;
    }

    /* remember how far we have computed */
    if (len < (size_t)n + 1)
        len = (size_t)n + 1;

    return cache[(size_t)n];
}
