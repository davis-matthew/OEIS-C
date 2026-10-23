/*
 * A180028  Eight white queens and one red queen on a 3 X 3 chessboard. G.f.:
 *          (1 + 3*x)/(1 - 6*x - 3*x^2).
 * https://oeis.org/A180028
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 6*a(n-1)+3*a(n-2) (a(0)=1, a(1)=9)
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 11, u64 n <= 23, u128 n <= 47,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static memoisation cache */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;   /* number of elements already computed */
    static idx_t cache_cap  = 0;   /* allocated capacity */

    /* initialise cache for the first two terms if needed */
    if (cache_size == 0) {
        cache_cap = 2;
        cache = (seq_t*)realloc(cache, cache_cap * sizeof(seq_t));
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)9;
        cache_size = 2;
    }

    /* grow the cache if the requested index is beyond current capacity */
    if (n + 1 > cache_cap) {
        cache_cap = (n + 1) * 2;  /* double the capacity to reduce reallocs */
        cache = (seq_t*)realloc(cache, cache_cap * sizeof(seq_t));
    }

    /* compute any missing values using the recurrence */
    while (cache_size <= n) {
        /* a(i) = 6*a(i-1) + 3*a(i-2) */
        seq_t prev  = cache[cache_size - 1];
        seq_t prev2 = cache[cache_size - 2];
        cache[cache_size] = (seq_t)6 * prev + (seq_t)3 * prev2;
        ++cache_size;
    }

    return cache[n];
}
