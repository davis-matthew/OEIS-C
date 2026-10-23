/*
 * A124806  Number of circular n-letter words over the alphabet {0,1,2,3,4}
 *          with adjacent letters differing by at most 2.
 * https://oeis.org/A124806
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 5*a(n-1) - 3*a(n-2) - 5*a(n-3) + a(n-4) + a(n-5) for n >
 *            5; Initial values: a(0)=1, a(1)=5, a(2)=19, a(3)=65, a(4)=247,
 *            a(5)=955
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 16, u64 n <= 32, u128 n <= 64,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;          /* Stores computed terms */
static size_t  cache_cap = 0;        /* Current capacity of cache */
static size_t  cache_len = 0;        /* Highest index that has been computed + 1 */

seq_t a(idx_t n) {
    /* Ensure we have the slot for n */
    if (cache_len == 0) {
        /* Allocate initial capacity */
        cache_cap = 16;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) return (seq_t)0;   /* Out of memory (should not happen) */
    }
    if ((size_t)n >= cache_cap) {
        /* Grow cache to at least n+1 elements */
        size_t new_cap = cache_cap * 2;
        if (new_cap <= (size_t)n) new_cap = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return (seq_t)0;   /* Out of memory */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Compute missing terms up to n */
    idx_t i;
    for (i = (idx_t)cache_len; i <= n; ++i) {
        seq_t val;
        if (i == 0)      val = (seq_t)1;
        else if (i == 1) val = (seq_t)5;
        else if (i == 2) val = (seq_t)19;
        else if (i == 3) val = (seq_t)65;
        else if (i == 4) val = (seq_t)247;
        else if (i == 5) val = (seq_t)955;
        else {
            /* Compute a(i) = 5*a(i-1) - 3*a(i-2) - 5*a(i-3) + a(i-4) + a(i-5) */
            seq_t t = (seq_t)5 * cache[i-1];
            t -= (seq_t)3 * cache[i-2];
            t -= (seq_t)5 * cache[i-3];
            t += cache[i-4];
            t += cache[i-5];
            val = t;
        }
        cache[i] = val;
    }
    cache_len = (size_t)n + 1;
    return cache[n];
}
