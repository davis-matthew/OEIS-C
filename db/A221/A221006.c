/*
 * A221006  The Wiener index of the nanostar dendrimer NS[n], defined
 *          pictorially in the Karbasioun-Ashrafi-Diudea reference.
 * https://oeis.org/A221006
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = -270 + 2^n*3510 + 4^n*(3888*n - 1944)
 * Range:     u8 none, u16 n <= 1, u32 n <= 8, u64 n <= 20, u128 n <= 20,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /* Handle the only case where the inner multiplier is negative
       to avoid unsigned underflow. */
    if (n == 0) return (seq_t)1296;

    /* Compute 2^n and 4^n = (2^n)^2. */
    seq_t pow2_n = pow2(n);
    seq_t pow4_n = pow2_n * pow2_n;

    /* 3510*2^n. */
    seq_t term1 = pow2_n * 3510;

    /* (3888*n - 1944) is positive for all n >= 1. */
    seq_t inner = (seq_t)n * 3888;
    inner -= (seq_t)1944;

    /* 4^n * (3888*n - 1944). */
    seq_t term2 = pow4_n * inner;

    /* Sum the positive parts and subtract the constant 270. */
    seq_t result = term1 + term2;
    result -= (seq_t)270;
    return result;
}
