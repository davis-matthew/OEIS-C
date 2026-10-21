/*
 * A063301  Dimension of the space of weight n cuspidal newforms for Gamma_1(
 *          28 ).
 * https://oeis.org/A063301
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = a(n-4) + a(n-6) - a(n-10) for n>13 (offset = 2); Initial
 *            values: a(2) = -1, a(3) = 8, a(4) = 22, a(5) = 36, a(6) = 48,
 *            a(7) = 62, a(8) = 74, a(9) = 92, a(10) = 100, a(11) = 118, a(12)
 *            = 126, a(13) = 146
 * Range:     i8 n <= 12, i16 n <= 51, i32 n <= 51, i64 n <= 51, i128 n <= 51,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>

seq_t a(idx_t n)
{
    if (n < 2)
        return 0;                     /* values below the offset are not defined */

    static seq_t *cache = NULL;
    static idx_t sz    = 0;

    /* initial values up to a(13) */
    static const seq_t init[14] = {
        0, 0, -1, 8, 22, 36, 48, 62, 74, 92,
        100, 118, 126, 146
    };

    /* allocate cache once with the size of the initial values */
    if (cache == NULL) {
        sz = 14;
        cache = (seq_t *)malloc(sz * sizeof(seq_t));
        if (!cache)
            return 0;
        memcpy(cache, init, sz * sizeof(seq_t));
    }

    /* extend cache if needed */
    if (n + 1 > sz) {
        idx_t new_sz = n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_sz * sizeof(seq_t));
        if (!tmp)                 /* allocation failed, return last known value */
            return cache[sz - 1];
        cache = tmp;
        /* compute new entries */
        for (idx_t i = sz; i < new_sz; ++i) {
            if (i <= 13)
                cache[i] = init[i];
            else
                cache[i] = cache[i - 4] + cache[i - 6] - cache[i - 10];
        }
        sz = new_sz;
    }

    return cache[n];
}
