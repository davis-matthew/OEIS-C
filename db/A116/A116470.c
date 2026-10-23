/*
 * A116470  All distinct Fibonacci and Lucas numbers.
 * https://oeis.org/A116470
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-2) + a(n-4) for n>6 a(0) = 0, a(1) = 1, a(2) = 2,
 *            a(3) = 3, a(4) = 4, a(5) = 5, a(6) = 7
 * Range:     u8 n <= 21, u16 n <= 44, u32 n <= 90, u64 n <= 182,
 *            u128 n <= 367, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    /* static cache for memoisation */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;   /* number of allocated slots */
    static idx_t  cache_size = 0;  /* number of values that have been computed */

    /* immediate return for the first seven terms */
    if (n < 7) {
        static const seq_t base[7] = { 0, 1, 2, 3, 4, 5, 7 };
        return base[n];
    }

    /* allocate or grow the cache if necessary */
    if (cache == NULL) {
        cache_cap = (n > 6) ? (n + 1) : 7;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
    } else if (cache_cap <= n) {
        size_t new_cap = cache_cap * 2;
        while (new_cap <= (size_t)n) new_cap *= 2;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (tmp == NULL) return 0;           /* fallback on allocation failure */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* initialise the first seven entries if not done yet */
    if (cache_size <= 6) {
        cache[0] = 0; cache[1] = 1; cache[2] = 2; cache[3] = 3;
        cache[4] = 4; cache[5] = 5; cache[6] = 7;
        cache_size = 7;
    }

    /* compute missing values up to n */
    if (cache_size <= n) {
        for (idx_t i = cache_size; i <= n; ++i) {
            /* a(i) = a(i-2) + a(i-4) for i > 6 */
            cache[i] = cache[i - 2] + cache[i - 4];
        }
        cache_size = n + 1;
    }

    return cache[(size_t)n];
}
