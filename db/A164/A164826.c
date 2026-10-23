/*
 * A164826  Sequences A087800 and A077416 interleaved.
 * https://oeis.org/A164826
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, less
 * Formula:   a(n) = 10*a(n-1)+a(n-2) for n odd, a(n) = a(n-1)+a(n-2) for n
 *            even; a(1)=2, a(2)=1
 * Range:     u8 n <= 6, u16 n <= 10, u32 n <= 18, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* before offset – silently handled */

    /* static cache of computed terms */
    static seq_t *cache = NULL;
    static idx_t cached = 0;     /* number of terms stored (>=0) */

    /* initialise cache with first two terms */
    if (cached == 0) {
        cache = (seq_t *)malloc(2 * sizeof(seq_t));
        if (!cache) return (seq_t)0;          /* allocation failure fallback */
        cache[0] = (seq_t)2;                  /* a(1) */
        cache[1] = (seq_t)1;                  /* a(2) */
        cached = 2;
    }

    /* if requested term already cached, return it */
    if (n <= (idx_t)cached) {
        return cache[n - 1];
    }

    /* grow the cache to size n and compute missing terms */
    seq_t *new_cache = (seq_t *)realloc(cache, n * sizeof(seq_t));
    if (!new_cache) return cache[n - 1];     /* allocation failure fallback */
    cache = new_cache;

    for (idx_t i = cached + 1; i <= n; ++i) {
        seq_t a_prev   = cache[i - 2];   /* a(i-1) */
        seq_t a_prev2  = cache[i - 3];   /* a(i-2) */
        seq_t val;
        if (i & 1U) {                    /* i is odd */
            val = (seq_t)10 * a_prev + a_prev2;
        } else {                         /* i is even */
            val = a_prev + a_prev2;
        }
        cache[i - 1] = val;
    }
    cached = n;
    return cache[n - 1];
}
