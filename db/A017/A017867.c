/*
 * A017867  Expansion of 1/(1 - x^8 - x^9).
 * https://oeis.org/A017867
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-8) + a(n-9) for n > 9; a(0)=1, a(1)=0, a(2)=0,
 *            a(3)=0, a(4)=0, a(5)=0, a(6)=0, a(7)=0, a(8)=1, a(9)=1
 * Range:     u8 n <= 91, u16 n <= 159, u32 n <= 296, u64 n <= 569,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#ifdef __cplusplus
#include <vector>
#endif

seq_t a(idx_t n)
{
#ifdef __cplusplus
    static std::vector<seq_t> cache;
    static idx_t computed_until = (idx_t)-1;

    if (computed_until == (idx_t)-1) {
        cache.resize(10);
        cache[0] = 1;
        for (idx_t i = 1; i < 8; ++i)
            cache[i] = 0;
        cache[8] = 1;
        cache[9] = 1;
        computed_until = 9;
    }

    if (n > computed_until) {
        cache.resize(n + 1);
        for (idx_t i = computed_until + 1; i <= n; ++i) {
            seq_t val = 0;
            if (i >= 8)
                val += cache[i - 8];
            if (i >= 9)
                val += cache[i - 9];
            cache[i] = val;
        }
        computed_until = n;
    }

    return cache[n];
#else
    /* C implementation */
    static seq_t *cache = NULL;
    static size_t capacity = 0;
    static idx_t computed_until = (idx_t)-1;

    if (computed_until == (idx_t)-1) {
        capacity = 10;
        cache = (seq_t *)malloc(capacity * sizeof(seq_t));
        if (!cache)
            return 0; /* allocation failure, return 0 */
        cache[0] = 1;
        for (idx_t i = 1; i < 8; ++i)
            cache[i] = 0;
        cache[8] = 1;
        cache[9] = 1;
        computed_until = 9;
    }

    if (n > computed_until) {
        size_t newcap = n + 1;
        if (newcap > capacity) {
            seq_t *tmp = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
            if (!tmp) {
                free(cache);
                cache = NULL;
                capacity = 0;
                computed_until = (idx_t)-1;
                return 0;
            }
            cache = tmp;
            capacity = newcap;
        }
        for (idx_t i = computed_until + 1; i <= n; ++i) {
            seq_t val = 0;
            if (i >= 8)
                val += cache[i - 8];
            if (i >= 9)
                val += cache[i - 9];
            cache[i] = val;
        }
        computed_until = n;
    }

    return cache[n];
#endif
}
