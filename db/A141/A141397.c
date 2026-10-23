/*
 * A141397  a(n) = 3*a(n-1) + A001651(n+1).
 * https://oeis.org/A141397
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (-1)^n/16 - 3*n/4 - 3/2 + 13*3^(n+1)/16
 * Range:     u8 n <= 1, u16 n <= 6, u32 n <= 16, u64 n <= 27, u128 n <= 27,
 *            big any n
 * Helpers:   pow3
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow3.h"

seq_t a(idx_t n) {
    /* compute 3^(n+1) */
    seq_t pow_term = pow3(n + 1);           /* 3^(n+1) */

    /* numerator: 13*3^(n+1) - 12*n - 24 + (-1)^n */
    seq_t num = pow_term * (seq_t)13;
    num -= (seq_t)(12 * n);
    num -= (seq_t)24;
    if ((n & 1) == 0)          /* (-1)^n : +1 when n even */
        num += (seq_t)1;
    else
        num -= (seq_t)1;

    /* divide by 16 to get a(n) */
    return num / (seq_t)16;
}
