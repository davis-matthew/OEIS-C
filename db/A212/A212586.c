/*
 * A212586  Nonnegative walks of length n on the x-axis starting at the origin
 *          using steps {1,0,-1} and visiting no point more than twice.
 * https://oeis.org/A212586
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, walk
 * Formula:   a(n) = 2*a(n-1) + a(n-2) - 2*a(n-3) + a(n-4) - a(n-6) with
 *            a(0)=1, a(1)=2, a(2)=6, a(3)=13, a(4)=29, a(5)=60
 * Range:     u8 n <= 6, u16 n <= 14, u32 n <= 29, u64 n <= 32, u128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cache_len = 0;

    /* Initialise cache on first use */
    if (!cache) {
        cache = (seq_t*)realloc(NULL, 6 * sizeof(seq_t));
        if (!cache) return 0;                 /* allocation failed */
        cache[0] = 1; cache[1] = 2; cache[2] = 6;
        cache[3] = 13; cache[4] = 29; cache[5] = 60;
        cache_len = 6;
    }

    /* Return stored value if already computed */
    if (n < cache_len) return cache[n];

    /* Grow cache to accommodate index n */
    size_t new_len = (size_t)n + 1;
    seq_t *tmp = (seq_t*)realloc(cache, new_len * sizeof(seq_t));
    if (!tmp) return 0;                     /* allocation failed */
    cache = tmp;

    /* Compute new terms using the recurrence */
    for (idx_t i = cache_len; i <= n; ++i) {
        seq_t val = 0;
        val += 2 * cache[i-1];
        val +=  cache[i-2];
        val -= 2 * cache[i-3];
        val +=  cache[i-4];
        val -=  cache[i-6];
        cache[i] = val;
    }

    cache_len = new_len;
    return cache[n];
}
