/*
 * A058182  a(n) = a(n-1)^2 + a(n-2) for n >= 2 with a(0) = 1 and a(1) = 0.
 * https://oeis.org/A058182
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, nice, eigen
 * Formula:   a(n) = a(n-1)^2 + a(n-2), a(0)=1, a(1)=0
 * Range:     u8 n <= 6, u16 n <= 7, u32 n <= 8, u64 n <= 9, u128 n <= 10,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;       /* static cache of computed terms   */
static idx_t cached_size = 0;     /* number of terms currently cached */

/* external linkage: function that returns a(n) for any n >= 0   */
seq_t a(idx_t n)
{
    /* initialise cache with first two terms */
    if (!cache)
    {
        cache = (seq_t*)malloc(2 * sizeof(seq_t));
        if (!cache) return 0;          /* allocation failure: return 0 (undefined) */
        cache[0] = (seq_t)1;           /* a(0) */
        cache[1] = (seq_t)0;           /* a(1) */
        cached_size = 2;
    }

    /* if requested term already cached, return it */
    if (n < cached_size)
        return cache[n];

    /* extend cache up to n */
    idx_t new_size = n + 1;
    seq_t *tmp = (seq_t*)realloc(cache, new_size * sizeof(seq_t));
    if (!tmp) return 0;                 /* allocation failure: return 0 (undefined) */
    cache = tmp;

    /* compute new terms iteratively, starting from cached_size */
    for (idx_t i = cached_size; i <= n; ++i)
    {
        seq_t prev  = cache[i - 1];  /* a(i-1) */
        seq_t prev2 = cache[i - 2];  /* a(i-2) */
        cache[i] = prev * prev + prev2;  /* a(i) = a(i-1)^2 + a(i-2) */
    }

    cached_size = new_size;
    return cache[n];
}
