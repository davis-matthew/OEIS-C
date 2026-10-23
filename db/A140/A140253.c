/*
 * A140253  a(2*n) = 2*(2*4^(n-1)-1) and a(2*n-1) = 2*4^(n-1)-1.
 * https://oeis.org/A140253
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(2*n) = 2*(2*4^(n-1)-1), a(2*n-1) = 2*4^(n-1)-1
 * Range:     i8 n <= 7, i16 n <= 15, i32 n <= 31, i64 n <= 63, i128 n <= 127,
 *            big any n
 * Helpers:   pow_int
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Note: a(0) = -1 is handled separately (offset 0). */

#include "../utils/pow_int.h"

seq_t a(idx_t n)
{
    /* Handle offset term a(0) */
    if (n == 0) {
        return (seq_t)-1;
    }

    /* If n is even:  a(2*m) = 4^m - 2 */
    if ((n & (idx_t)1) == 0) {
        idx_t m = n / 2;                    /* m >= 1 */
        seq_t p = pow_int((seq_t)4, m);     /* 4^m */
        return p - (seq_t)2;
    }

    /* If n is odd:  a(2*m-1) = 2*4^(m-1) - 1 */
    idx_t m = (n + 1) / 2;                  /* m >= 1 */
    seq_t p = pow_int((seq_t)4, m - 1);     /* 4^(m-1) */
    return (seq_t)2 * p - (seq_t)1;
}
