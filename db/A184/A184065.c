/*
 * A184065  Number of (n+1) X 4 binary arrays with rows and columns in
 *          nondecreasing order and with no 2 X 2 subblock sum differing from
 *          a horizontal or vertical neighbor subblock sum by more than one.
 * https://oeis.org/A184065
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 4*a(n-1) - 6*a(n-2) + 4*a(n-3) - a(n-4) for n > 13, with
 *            initial values: a(1)=20, a(2)=36, a(3)=130, a(4)=382, a(5)=924,
 *            a(6)=1884, a(7)=3463, a(8)=5800, a(9)=9065, a(10)=13397,
 *            a(11)=18940, a(12)=25837, a(13)=34232
 * Range:     u8 n <= 3, u16 n <= 15, u32 n <= 200, u64 n <= 200, u128 n <= 14,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#ifdef __cplusplus
#include <vector>
#endif

static const seq_t init[14] = {
    (seq_t)0,   /* dummy 0‑th element */
    (seq_t)20,  (seq_t)36,   (seq_t)130,
    (seq_t)382, (seq_t)924,   (seq_t)1884,
    (seq_t)3463,(seq_t)5800,  (seq_t)9065,
    (seq_t)13397,(seq_t)18940,(seq_t)25837,(seq_t)34232
};

seq_t a(idx_t n)
{
    /* small n handled directly */
    if (n <= 13) return init[(int)n];

#ifdef __cplusplus
    /* C++ build – use std::vector to construct objects */
    static std::vector<seq_t> cache;
    static size_t cache_max = 0;

    if (cache.empty()) {
        cache.resize(14);                 /* indices 0 … 13 */
        for (int i = 1; i <= 13; ++i) cache[i] = init[i];
        cache_max = 13;
    }

    if (n > cache_max) {
        if (cache.size() <= n) cache.resize(n + 1);
        for (idx_t i = cache_max + 1; i <= n; ++i) {
            cache[i] = 4 * cache[i - 1] + 4 * cache[i - 3]
                        - (6 * cache[i - 2] + cache[i - 4]);
        }
        cache_max = n;
    }
    return cache[(size_t)n];
#else
    /* C build – use malloc/realloc (seq_t is a plain integer) */
    static seq_t *cache = NULL;
    static size_t cap = 0;
    static size_t cached_max = 0;

    if (cache == NULL) {
        cap = 14;                      /* need indices 0 … 13 */
        cache = (seq_t *)malloc(sizeof(seq_t) * cap);
        for (int i = 1; i <= 13; ++i) cache[i] = init[i];
        cached_max = 13;
    } else if ((size_t)n + 1 > cap) {
        size_t new_cap = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, sizeof(seq_t) * new_cap);
        if (tmp == NULL) {            /* allocation failed – return 0 */
            return (seq_t)0;
        }
        cache = tmp;
        cap = new_cap;
    }

    if ((size_t)n > cached_max) {
        for (idx_t i = cached_max + 1; i <= n; ++i) {
            cache[i] = 4 * cache[i - 1] + 4 * cache[i - 3]
                        - (6 * cache[i - 2] + cache[i - 4]);
        }
        cached_max = (size_t)n;
    }
    return cache[(size_t)n];
#endif
}
