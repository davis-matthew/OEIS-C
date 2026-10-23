/*
 * A172609  a(n) = (8n)!/(8!^n).
 * https://oeis.org/A172609
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (8n)!/(8!^n)
 * Range:     u8 n <= 0, u16 n <= 1, u32 n <= 2, u64 n <= 4, u128 n <= 6,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    seq_t res = 1;
    for (idx_t i = 1; i <= n; ++i) {
        seq_t mult = 1;
        seq_t step = (seq_t)(8) * (seq_t)i;  /* 8 * i as seq_t */

        for (int j = 0; j < 8; ++j) {
            mult *= (step - (seq_t)j);            /* multiply by (8*i - j) */
            mult /= (seq_t)(j + 1);              /* divide by (j+1)  */
        }
        res *= mult;
    }
    return res;
}
