/*
 * A225311  Number of 4 X n -1,1 arrays such that the sum over i=1..4,j=1..n
 *          of i*x(i,j) is zero and rows are nondecreasing (ways to put n
 *          thrusters pointing east or west at each of 4 positions 1..n
 *          distance from the hinge of a south-pointing gate without turning
 *          the gate).
 * https://oeis.org/A225311
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1)+a(n-2)-2*a(n-5)+a(n-8)+a(n-9)-a(n-10) for n ≥ 11;
 *            a(1)=2, a(2)=7, a(3)=16, a(4)=31, a(5)=52, a(6)=83, a(7)=122,
 *            a(8)=175, a(9)=238, a(10)=317
 * Range:     u8 n <= 9, u16 n <= 64, u32 n <= 210, u64 n <= 210,
 *            u128 n <= 210, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

static seq_t *cache = NULL;          /* cache[n-1] holds a(n) */
static size_t cache_len = 0;         /* number of entries in cache */

/* first ten terms of the sequence, used as initialization */
static const seq_t init[10] = {2, 7, 16, 31, 52,
                               83, 122, 175, 238, 317};

/* Return the n-th term of the sequence (n ≥ 1). */
seq_t a(idx_t n)
{
    if (n == 0)     /* offset starts at 1 – undefined below */
        return 0;

    size_t target = (size_t)n;          /* elements required */

    if (cache_len < target) {
        /* grow the cache to hold at least target elements */
    #if defined(__cplusplus)
        seq_t *tmp = reinterpret_cast<seq_t*>(
            realloc(cache, target * sizeof(seq_t)));
    #else
        seq_t *tmp = (seq_t*)realloc(cache,
            target * sizeof(seq_t));
    #endif
        if (tmp == NULL)   /* allocation failure – return 0 */
            return 0;
        cache = tmp;

        /* compute any new terms */
        for (size_t i = cache_len; i < target; ++i) {
            if (i < 10)
                cache[i] = init[i];
            else
                cache[i] = cache[i-1] + cache[i-2]
                           - 2*cache[i-5] + cache[i-8]
                           + cache[i-9] - cache[i-10];
        }
        cache_len = target;
    }

    return cache[n-1];
}
