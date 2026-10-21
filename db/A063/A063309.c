/*
 * A063309  Dimension of the space of weight n cuspidal newforms for Gamma_1(
 *          36 ).
 * https://oeis.org/A063309
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = a(n-4) + a(n-6) - a(n-10) for n > 13, a(2) = -1, a(3) =
 *            13, a(4) = 28, a(5) = 45, a(6) = 63, a(7) = 78, a(8) = 96, a(9)
 *            = 111, a(10) = 129, a(11) = 144, a(12) = 164, a(13) = 176
 * Range:     i8 n <= 9, i16 n <= 51, i32 n <= 51, i64 n <= 51, i128 n <= 51,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

seq_t a(idx_t n)
{
    if (n < 2)
        return 0; /* undefined for n < 2, but return 0 */

    static seq_t *vals = NULL;
    static idx_t size = 0;

    /* initialize base values up to n = 13 */
    if (!vals) {
        size = 14; /* indices 0..13 */
        vals = (seq_t*)calloc(size, sizeof(seq_t));
        /* set known values */
        vals[2] = (seq_t)-1;
        vals[3] = (seq_t)13;
        vals[4] = (seq_t)28;
        vals[5] = (seq_t)45;
        vals[6] = (seq_t)63;
        vals[7] = (seq_t)78;
        vals[8] = (seq_t)96;
        vals[9] = (seq_t)111;
        vals[10] = (seq_t)129;
        vals[11] = (seq_t)144;
        vals[12] = (seq_t)164;
        vals[13] = (seq_t)176;
    }

    /* extend memoization array if needed */
    if (n >= size) {
        idx_t new_size = n + 1;
        seq_t *tmp = (seq_t*)realloc(vals, new_size * sizeof(seq_t));
        if (!tmp) /* allocation failed */
            return 0;
        vals = tmp;
        memset(vals + size, 0, (new_size - size) * sizeof(seq_t));
        /* compute new values iteratively */
        for (idx_t i = size; i <= n; ++i) {
            if (i <= 13) {
                /* base values already set */
                continue;
            }
            vals[i] = vals[i-4] + vals[i-6] - vals[i-10];
        }
        size = new_size;
    }

    return vals[n];
}
