/*
 * A141265  Concatenate first n factorials in reverse order.
 * https://oeis.org/A141265
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = concat_{k=1}^{n+1} k! (reverse order, largest first)
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 4, u64 n <= 6, u128 n <= 9,
 *            big any n
 * Helpers:   digit_len, pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow10.h"
#include "../utils/digit_len.h"

seq_t a(idx_t n)
{
    seq_t res  = (seq_t)1;   /* a(0) = 1! */
    seq_t fact = (seq_t)1;   /* factorial of (0+1) = 1! */

    for (idx_t i = 0; i < n; ++i) {
        /* factorial of (i+2)! = previous fact * (i+2) */
        fact = fact * (seq_t)(i + 2);

        /* prepend new factorial to the current result */
        idx_t shift = (idx_t)digit_len(res);            /* decimal digits of previous result */
        res = fact * pow10(shift) + res;
    }
    return res;
}
