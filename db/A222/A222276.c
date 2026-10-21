/*
 * A222276  Number of n X 3 0..5 arrays with no entry increasing mod 6 by 5
 *          rightwards or downwards, starting with upper left zero.
 * https://oeis.org/A222276
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 10, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(1)=25, a(2)=2205,
   a(n) = 92*a(n-1) - 335*a(n-2)   (n >= 3)  */

#include <stdlib.h>

static seq_t *cache = NULL;
static size_t cache_size = 0;

seq_t a(idx_t n)
{
    if (n < 1)
        return 0;                     /* undefined for n < 1 */

    /* initialise cache with first two values if necessary */
    if (cache_size < 2) {
        size_t newsize = 2;
        cache = (seq_t *)realloc(cache, newsize * sizeof(seq_t));
        cache[0] = 25;              /* a(1) */
        cache[1] = 2205;            /* a(2) */
        cache_size = newsize;
    }

    if ((size_t)n <= cache_size)
        return cache[n-1];          /* value already computed */

    /* grow cache to accommodate n */
    size_t newsize = (size_t)n;
    seq_t *tmp = (seq_t *)realloc(cache, newsize * sizeof(seq_t));
    if (!tmp)                         /* allocation failure */
        return 0;
    cache = tmp;

    /* compute missing terms using the recurrence */
    for (idx_t i = cache_size; i < n; ++i) {
        /* i indexes 0‑based: cache[i] corresponds to a(i+1) */
        cache[i] = 92 * cache[i-1] - 335 * cache[i-2];
    }
    cache_size = newsize;
    return cache[n-1];
}
