/*
 * A008044  Coordination sequence T2 for Zeolite Code ATV.
 * https://oeis.org/A008044
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1)+a(n-8)-a(n-9)+a(n-10)-a(n-11)-a(n-18)+a(n-19) for
 *            n >= 20 Initial values: a(0)=1, a(1)=4, a(2)=12, a(3)=25,
 *            a(4)=42, a(5)=61, a(6)=88, a(7)=122, a(8)=160, a(9)=200,
 *            a(10)=246, a(11)=299, a(12)=358, a(13)=417, a(14)=480,
 *            a(15)=550, a(16)=628, a(17)=710, a(18)=796, a(19)=887
 * Range:     u8 n <= 10, u16 n <= 163, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    /* Static cache for memoisation */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;
    static size_t cache_cap = 0;

    /* Initial values 0..19 */
    static const seq_t init[20] = {
        (seq_t)1,  (seq_t)4,  (seq_t)12, (seq_t)25, (seq_t)42,
        (seq_t)61, (seq_t)88, (seq_t)122, (seq_t)160, (seq_t)200,
        (seq_t)246, (seq_t)299, (seq_t)358, (seq_t)417, (seq_t)480,
        (seq_t)550, (seq_t)628, (seq_t)710, (seq_t)796, (seq_t)887
    };

    /* Fast return when n < 20 */
    if (n < 20) {
        return init[(size_t)n];
    }

    /* Initialise cache to hold initial 20 terms */
    if (cache_len == 0) {
        cache_cap = 20;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) return (seq_t)0; /* allocation failed */
        for (size_t i = 0; i < 20; ++i) {
            cache[i] = init[i];
        }
        cache_len = 20;
    }

    /* Grow cache capacity if necessary */
    if ((size_t)n + 1 > cache_cap) {
        size_t new_cap = cache_cap;
        while ((size_t)n + 1 > new_cap) {
            new_cap *= 2;
        }
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return (seq_t)0; /* allocation failed */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Compute missing terms up to n */
    if ((size_t)n + 1 > cache_len) {
        for (size_t k = cache_len; k <= (size_t)n; ++k) {
            /* recurrence uses indices: k-1, k-8, k-9, k-10, k-11, k-18, k-19 */
            seq_t val = cache[k - 1] + cache[k - 8] + cache[k - 10] + cache[k - 19];
            val -= cache[k - 9];
            val -= cache[k - 11];
            val -= cache[k - 18];
            cache[k] = val;
        }
        cache_len = (size_t)n + 1;
    }

    return cache[(size_t)n];
}
