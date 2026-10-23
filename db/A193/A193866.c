/*
 * A193866  Even pentagonal numbers divided by 2.
 * https://oeis.org/A193866
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 1/16*(1-3*(-1)^n+12*n)*(1-(-1)^n+4*n)
 * Range:     u8 n <= 8, u16 n <= 147, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    seq_t nn = (seq_t)n;
    seq_t t1, t2;
    /* (-1)^n =  1 when n is even, -1 when n is odd.
       Avoid negative numbers in unsigned arithmetic. */
    if ((n & (idx_t)1) == 0) {          /* n even */
        t1 = (seq_t)12 * nn - (seq_t)2; /* 12*n - 2   */
        t2 = (seq_t)4  * nn;            /* 4*n        */
    } else {                            /* n odd */
        t1 = (seq_t)12 * nn + (seq_t)4; /* 12*n + 4   */
        t2 = (seq_t)4  * nn + (seq_t)2; /* 4*n + 2    */
    }
    return (t1 * t2) / (seq_t)16;
}
