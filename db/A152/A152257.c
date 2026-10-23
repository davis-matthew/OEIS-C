/*
 * A152257  a(n) = (3^n - 1)^2*(3^n + 1)/16.
 * https://oeis.org/A152257
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (3^n - 1)^2*(3^n + 1)/16
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 7, u64 n <= 14, u128 n <= 27,
 *            big any n
 * Helpers:   pow3
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Offset: 0 */

#include "../utils/pow3.h"   /* provides seq_t pow3(idx_t n) */

seq_t a(idx_t n)
{
    /* Compute 3^n. */
    seq_t t = pow3(n);           /* t = 3^n */

    /* a = t - 1, b = t + 1.  Both are even since t is odd. */
    seq_t a_val = t - 1;
    seq_t b_val = t + 1;

    /* Half the even numbers. */
    seq_t a_half = a_val / 2;    /* (t-1)/2  */
    seq_t b_half = b_val / 2;    /* (t+1)/2  */

    /* One of a_half, b_half is even, the other is odd. */
    if ((a_half % 2) == 0) {
        /* a_half is even; let temp = a_half / 2, o = b_half (odd). */
        seq_t temp = a_half / 2;
        seq_t o    = b_half;
        /* (t-1)^2 * (t+1) / 16 = 2 * (temp^2) * o */
        seq_t res = temp * temp;
        res = res * o;
        res = res * 2;
        return res;
    } else {
        /* b_half is even; let temp = b_half / 2, o = a_half (odd). */
        seq_t temp = b_half / 2;
        seq_t o    = a_half;
        /* (t-1)^2 * (t+1) / 16 = (o^2) * temp */
        seq_t res = o * o;
        res = res * temp;
        return res;
    }
}
