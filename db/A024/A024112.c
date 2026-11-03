/*
 * A024112  a(n) = 9^n - n^11.
 * https://oeis.org/A024112
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = 9^n - n^11
 * Range:     i8 n <= 1, i16 n <= 2, i32 n <= 7, i64 n <= 19, i128 n <= 40,
 *            big any n
 * Validated: standard, pass, 33 terms through n = 291 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/mem.h"

seq_t a(idx_t n)
{
    /* Cache of 9^k for k <= current max */
    static seq_t *pow9 = NULL;
    static size_t pow9_len = 0;

    /* initialise cache for n = 0 */
    if (pow9_len == 0) {
        pow9 = mem_new(seq_t, 1);
        pow9[0] = (seq_t)1;
        pow9_len = 1;
    }

    /* extend cache up to n if necessary */
    if (pow9_len <= n) {
        size_t new_len = n + 1;
        pow9 = mem_resize(seq_t, pow9, new_len);
        for (size_t i = pow9_len; i < new_len; ++i) {
            pow9[i] = pow9[i - 1] * (seq_t)9;
        }
        pow9_len = new_len;
    }

    seq_t nine_pow = pow9[n];

    /* compute n^11 by repeated multiplication */
    seq_t np = (seq_t)n;
    seq_t n11 = (seq_t)1;
    for (int i = 0; i < 11; ++i) {
        n11 *= np;
    }

    return nine_pow - n11;
}
