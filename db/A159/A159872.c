/*
 * A159872  Numerator of Hermite(n, 8/23).
 * https://oeis.org/A159872
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, frac
 * Formula:   a(n) = 23^n * Hermite(n, 8/23)
 * Range:     i8 n <= 1, i16 n <= 2, i32 n <= 5, i64 n <= 10, i128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n) {
    /* Memoised values */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static size_t cache_len = 0;

    size_t idx = (size_t)n;                   /* safe: n <= 10000 in tests */

    /* Grow cache if necessary */
    if (idx >= cache_len) {
        size_t needed = idx + 1;
        if (cache_cap < needed) {
            size_t new_cap = cache_cap ? cache_cap : 16;
            while (new_cap < needed) new_cap <<= 1;
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp) exit(EXIT_FAILURE);    /* allocation failure */
            cache = tmp;
            cache_cap = new_cap;
        }

        /* initialise first terms if not present */
        if (cache_len == 0) {
            cache[0] = (seq_t)1;
            cache_len = 1;
        }
        if (cache_len == 1 && needed > 1) {
            cache[1] = (seq_t)16;
            cache_len = 2;
        }

        /* compute further terms using recurrence:
           a(n) = 16*a(n-1) - 1058*(n-1)*a(n-2)  */
        for (size_t k = cache_len; k <= idx; ++k) {
            seq_t mult = (seq_t)1058 * (seq_t)(k - 1);
            seq_t term1 = (seq_t)16 * cache[k - 1];
            seq_t term2 = mult * cache[k - 2];
            cache[k] = term1 - term2;
        }
        cache_len = needed;
    }
    return cache[idx];
}
