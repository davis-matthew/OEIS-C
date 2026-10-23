/*
 * A019439  Number of ways of tiling a 2 X n rectangle with dominoes and
 *          trominoes.
 * https://oeis.org/A019439
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*a(n-1) + 3*a(n-3) + 2*a(n-4) + a(n-5) - a(n-6) initial
 *            values: a(0)=1, a(1)=1, a(2)=2, a(3)=6, a(4)=17, a(5)=43
 * Range:     u8 n <= 6, u16 n <= 12, u32 n <= 24, u64 n <= 47, u128 n <= 94,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;
static size_t cache_len = 0;

seq_t a(idx_t n)
{
    /* Fast return for first six terms */
    if (n < 6) {
        static const seq_t init[6] = {1, 1, 2, 6, 17, 43};
        return init[(size_t)n];
    }

    /* Allocate or enlarge cache as needed */
    size_t target = (size_t)n + 1;
    if (cache_len < target) {
        seq_t *new_cache = (seq_t *)realloc(cache, target * sizeof(seq_t));
        if (!new_cache) return (seq_t)0;           /* fall back on failure */
        cache = new_cache;
        /* Initialize the first six entries if this is the first allocation */
        if (cache_len == 0) {
            cache[0] = 1;
            cache[1] = 1;
            cache[2] = 2;
            cache[3] = 6;
            cache[4] = 17;
            cache[5] = 43;
            cache_len = 6;
        }
    }

    /* Compute missing entries up to n */
    for (size_t i = cache_len; i <= n; ++i) {
        cache[i] = 2*cache[i-1]               // 2*a(n-1)
                  + 3*cache[i-3]             // 3*a(n-3)
                  + 2*cache[i-4]             // 2*a(n-4)
                  +  cache[i-5]              //   a(n-5)
                  -  cache[i-6];              // -  a(n-6)
    }
    cache_len = target;
    return cache[(size_t)n];
}
