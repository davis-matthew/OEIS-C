/*
 * A041047  Denominators of continued fraction convergents to sqrt(29).
 * https://oeis.org/A041047
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy
 * Formula:   a(n) = 140*a(n-5) + a(n-10) Initial terms: a(0)=1, a(1)=2,
 *            a(2)=3, a(3)=5, a(4)=13, a(5)=135, a(6)=283, a(7)=418, a(8)=701,
 *            a(9)=1820
 * Range:     u8 n <= 5, u16 n <= 12, u32 n <= 23, u64 n <= 44, u128 n <= 89,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for memoisation */
    static seq_t *cache = NULL;
    static size_t cache_capacity = 0;
    static size_t cache_len = 0;
    static const seq_t base[10] = {
        (seq_t)1,  (seq_t)2,  (seq_t)3,  (seq_t)5,
        (seq_t)13, (seq_t)135, (seq_t)283, (seq_t)418,
        (seq_t)701, (seq_t)1820
    };

    /* handle small indices directly */
    if (n < 10)
        return base[n];

    /* initialise cache on first call */
    if (cache == NULL) {
        cache_capacity = 10;
        cache = (seq_t *)malloc(cache_capacity * sizeof(seq_t));
        if (!cache) return 0;                      /* out of memory: return 0 */
        for (size_t i = 0; i < 10; ++i) cache[i] = base[i];
        cache_len = 10;
    }

    /* grow cache if required */
    if ((idx_t)cache_len <= n) {
        size_t needed = (size_t)(n + 1);
        if (needed > cache_capacity) {
            size_t new_cap = cache_capacity;
            while (new_cap < needed)
                new_cap = new_cap * 2;
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp) return 0;                    /* out of memory */
            cache = tmp;
            cache_capacity = new_cap;
        }

        /* compute terms up to n inclusively */
        for (idx_t i = (idx_t)cache_len; i <= n; ++i) {
            size_t idx = (size_t)i;
            cache[idx] = (seq_t)140 * cache[idx - 5] + cache[idx - 10];
        }
        cache_len = (size_t)(n + 1);
    }

    return cache[(size_t)n];
}
