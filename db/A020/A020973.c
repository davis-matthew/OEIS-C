/*
 * A020973  Expansion of 1/((1-7*x)*(1-10*x)*(1-11*x)).
 * https://oeis.org/A020973
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 28*a(n-1) - 257*a(n-2) + 770*a(n-3) for n>=3 a(0) = 1,
 *            a(1) = 28, a(2) = 527
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 8, u64 n <= 17, u128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Static cache for memoisation */
    static seq_t *cache   = NULL;
    static size_t cache_cap = 0;   /* allocated cache size    */
    static size_t cache_size = 0;  /* number of terms stored  */

    /* Return cached value if available */
    if (n < cache_size) {
        return cache[n];
    }

    /* Initialise cache on first use */
    if (cache_size == 0) {
        cache_cap = (n < 3 ? 3 : n + 1);
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) return 0;          /* allocation failed */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)28;
        cache[2] = (seq_t)527;
        cache_size = 3;
        if (n < 3) {
            return cache[n];
        }
    }

    /* Grow cache if necessary */
    if (n >= cache_cap) {
        size_t new_cap = n + 1;
        if (new_cap < cache_cap * 2) {
            new_cap = cache_cap * 2;   /* amortised doubling */
        }
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return 0;          /* reallocation failed */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Compute missing terms using the recurrence */
    for (idx_t i = (idx_t)cache_size; i <= n; ++i) {
        seq_t a_n1 = cache[i - 1];
        seq_t a_n2 = cache[i - 2];
        seq_t a_n3 = cache[i - 3];
        cache[i] = (28 * a_n1) - (257 * a_n2) + (770 * a_n3);
    }
    cache_size = (size_t)n + 1;
    return cache[n];
}
