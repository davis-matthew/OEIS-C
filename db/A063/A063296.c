/*
 * A063296  Dimension of the space of weight n cuspidal newforms for Gamma_1(
 *          23 ).
 * https://oeis.org/A063296
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = a(n-4) + a(n-6) - a(n-10) for n > 13. Offset 2: a(2) = -1
 *            a(3) = 12 a(4) = 33 a(5) = 55 a(6) = 77 a(7) = 99 a(8) = 121
 *            a(9) = 143 a(10) = 165 a(11) = 187 a(12) = 209 a(13) = 229
 * Range:     i8 n <= 8, i16 n <= 50, i32 n <= 50, i64 n <= 50, i128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    if (n < 2) return 0; /* behavior undefined below the offset */

    static seq_t *cache = NULL;          /* stores a(2), a(3), ... */
    static size_t cache_len = 0;         /* number of stored terms */

    /* Initial values for n = 2 .. 13 */
    static const seq_t base[12] = {
        (seq_t)-1, 12, 33, 55, 77, 99, 121, 143, 165, 187, 209, 229
    };

    size_t idx = (size_t)(n - 2);        /* index in cache */

    if (cache_len <= idx) {
        /* Need to extend the cache */
        size_t new_len = idx + 1;
        seq_t *new_cache = (seq_t*)realloc(cache, new_len * sizeof(seq_t));
        if (!new_cache) return 0;        /* allocation failed: fallback */

        cache = new_cache;
        for (size_t i = cache_len; i < new_len; ++i) {
            idx_t cur_n = (idx_t)(i + 2);
            if (cur_n <= 13) {
                /* copy base values */
                cache[i] = base[cur_n - 2];
            } else {
                /* recurrence: a(n) = a(n-4) + a(n-6) - a(n-10) */
                cache[i] = cache[i - 4] + cache[i - 6] - cache[i - 10];
            }
        }
        cache_len = new_len;
    }

    return cache[idx];
}
