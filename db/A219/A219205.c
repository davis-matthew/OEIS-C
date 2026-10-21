/*
 * A219205  a(n) = 3^(n-1)*(3^n - 1), n >= 0.
 * https://oeis.org/A219205
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3^(n-1)*(3^n - 1)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 10, u64 n <= 19, u128 n <= 19,
 *            big any n
 * Helpers:   pow3
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow3.h"

seq_t a(idx_t n)
{
    if (n == 0) {
        return (seq_t)0;
    }
    seq_t p_prev = pow3(n - 1);          /* 3^(n-1) */
    seq_t p_n    = p_prev * (seq_t)3;    /* 3^n     */
    return p_prev * (p_n - (seq_t)1);    /* 3^(n-1)*(3^n-1) */
}
