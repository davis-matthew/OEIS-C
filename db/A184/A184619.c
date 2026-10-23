/*
 * A184619  a(n) = floor((n-h)*s+h), where s=2+sqrt(2) and h=1/3; complement
 *          of A184618.
 * https://oeis.org/A184619
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor((n-1/3)*(2+sqrt(2)) + 1/3)
 * Range:     u8 n <= 3, u16 n <= 22, u32 n <= 5983, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    /* Convert the index to seq_t */
    seq_t N = (seq_t)n;

    /* Compute 2 * N^2 */
    seq_t N2 = N * N;
    seq_t M  = (seq_t)2 * N2;

    /* t = floor_sqrt(2 * N^2) */
    seq_t t = floor_sqrt(M);

    /* diff = M - t^2 (distance to the next perfect square) */
    seq_t diff = M - t * t;

    /* L  = 9 * diff
       A  = 6 * t + 3
       B  = 6 * t + 2   (coefficient of sqrt(2) in the inequality)
       left_minus = L - A  (>= 0 if we are in the range where
                            the inequality is meaningful)
    */
    seq_t L = diff * (seq_t)9;
    seq_t A = t * (seq_t)6 + (seq_t)3;
    seq_t B = t * (seq_t)6 + (seq_t)2;

    seq_t subtract_flag = 0;

    if (L < A) {
        /* 9*diff < 6*t+3  =>  f < c  => subtract 1 */
        subtract_flag = 1;
    } else {
        seq_t left_minus = L - A;                     /* >= 0 */
        seq_t left_sq    = left_minus * left_minus;    /* (L-A)^2   */
        seq_t B_sq       = B * B;                      /* B^2       */
        seq_t right_sq   = B_sq * (seq_t)2;            /* 2*B^2     */

        if (left_sq < right_sq)
            subtract_flag = 1;   /* f < c, subtract 1 */
        /* else f >= c, subtract_flag remains 0 */
    }

    /* floor(n*s) = 2*n + floor(n*sqrt(2)) = 2*N + t */
    seq_t floor_ns = (seq_t)2 * N + t;

    return floor_ns - subtract_flag;
}
