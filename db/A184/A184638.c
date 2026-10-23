/*
 * A184638  floor(nr+h), where r=sqrt(3), h=-1/2; complement of A184653.
 * https://oeis.org/A184638
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor(n*sqrt(3) - 1/2)
 * Range:     u8 n <= 4, u16 n <= 73, u32 n <= 116, u64 n <= 116,
 *            u128 n <= 116, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/floor_sqrt.h"

seq_t a(idx_t n) {
    /* t = floor(n*sqrt(3)) = floor(sqrt(3*n*n)) */
    seq_t nn = (seq_t)n;
    seq_t v = (seq_t)3 * nn * nn;
    seq_t t = floor_sqrt(v);

    /* Determine whether the fractional part of n*sqrt(3) is < 0.5
       by comparing 12*n^2 with (2*t+1)^2. */
    seq_t left  = (seq_t)12 * nn * nn;
    seq_t two_t_plus1 = (seq_t)2 * t + 1;
    seq_t right = two_t_plus1 * two_t_plus1;

    if (left < right)
        return t - 1;
    else
        return t;
}
