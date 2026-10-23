/*
 * A176126  Numerator of -A127276(n)/A001788(n).
 * https://oeis.org/A176126
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, frac
 * Formula:   a(n) = 3*a(n-4) - 3*a(n-8) + a(n-12) (for n >= 12) Base values:
 *            a(0) = -1, a(1) = -1, a(2) = 1, a(3) = 2, a(4) = 4, a(5) = 13,
 *            a(6) = 19, a(7) = 13, a(8) = 17, a(9) = 43, a(10) = 53, a(11) =
 *            32
 * Range:     i8 n <= 16, i16 n <= 79, i32 n <= 79, i64 n <= 79, i128 n <= 79,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* static cache for memoisation */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;
    static size_t cache_cap = 0;

    /* enlarge cache if required */
    if (n >= cache_cap) {
        size_t new_cap = cache_cap ? (cache_cap * 2) : 16;
        while (new_cap <= (size_t)n) new_cap *= 2;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return 0;          /* out‑of‑memory – unlikely in tests */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* initialise first 12 terms */
    const seq_t init[12] = {
        (seq_t)-1, (seq_t)-1, (seq_t)1, (seq_t)2,
        (seq_t)4,  (seq_t)13, (seq_t)19, (seq_t)13,
        (seq_t)17, (seq_t)43, (seq_t)53, (seq_t)32
    };
    if (cache_size < 12) {
        size_t upto = 12;
        if (upto > cache_cap) upto = cache_cap;
        for (size_t i = cache_size; i < upto; ++i) {
            cache[i] = init[i];
        }
        cache_size = upto;
    }

    /* compute missing terms up to n */
    for (idx_t i = cache_size; i <= n; ++i) {
        seq_t val;
        if (i < 12) {
            val = init[i];
        } else {
            val = 3 * cache[i-4] - 3 * cache[i-8] + cache[i-12];
        }
        cache[i] = val;
    }
    if (cache_size <= n) cache_size = n + 1;   /* keep cache size >= n+1 */

    return cache[n];
}
