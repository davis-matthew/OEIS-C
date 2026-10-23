/*
 * A017834  Expansion of 1/(1-x^4-x^5-x^6-x^7-x^8-x^9-x^10-x^11-x^12).
 * https://oeis.org/A017834
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 23, u16 n <= 41, u32 n <= 76, u64 n <= 147,
 *            u128 n <= 289, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: a(n) = a(n-4)+a(n-5)+a(n-6)+a(n-7)+a(n-8)+a(n-9)+
 *               a(n-10)+a(n-11)+a(n-12)  for n > 11
 * Initial values: 
 * a(0)=1, a(1)=0, a(2)=0, a(3)=0,
 * a(4)=1, a(5)=1, a(6)=1, a(7)=1, a(8)=2, a(9)=3, a(10)=4,
 * a(11)=5, a(12)=7
 */

#include <stdlib.h>
#include <stdint.h>

static seq_t *cache = NULL;          /* memoized terms */
static idx_t  cached_max = 0;        /* highest index computed */

/* allocate and initialise the first 13 terms */
static void init_cache(void) {
    /* allocate space for 13 terms */
    cache = (seq_t *)malloc(13 * sizeof(seq_t));
    if (!cache) return;            /* allocation failure – leave cache NULL */

    cache[0] = 1;
    cache[1] = 0; cache[2] = 0; cache[3] = 0;
    cache[4] = 1;
    cache[5] = 1;
    cache[6] = 1;
    cache[7] = 1;
    cache[8] = 2;
    cache[9] = 3;
    cache[10] = 4;
    cache[11] = 5;
    cache[12] = 7;
    cached_max = 12;
}

seq_t a(idx_t n) {
    /* ensure the cache is initialised */
    if (!cache) init_cache();
    if (!cache) return 0;          /* still NULL: allocation failed */

    /* if the requested index is already cached, return it */
    if (n <= cached_max) return cache[n];

    /* enlarge the cache to hold at least n+1 terms */
    size_t new_size = (size_t)(n + 1);
    seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
    if (!tmp) {                    /* realloc failed – return zero */
        return 0;
    }
    cache = tmp;

    /* compute missing terms up to n */
    for (idx_t i = cached_max + 1; i <= n; ++i) {
        seq_t sum = 0;
        for (int k = 4; k <= 12; ++k) {
            if (i >= (idx_t)k) {
                sum += cache[i - k];
            }
        }
        cache[i] = sum;
    }
    cached_max = n;
    return cache[n];
}
