/*
 * A221969  Number of -n..n arrays of length 6 with the sum ahead of each
 *          element differing from the sum following that element by n or
 *          less.
 * https://oeis.org/A221969
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (128*n^6 + 384*n^5 + 560*n^4 + 480*n^3 + 272*n^2 + 96*n +
 *            15)/15
 * Range:     u8 none, u16 n <= 2, u32 n <= 17, u64 n <= 35, u128 n <= 35,
 *            big any n
 * Helpers:   mul_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/mul_small.h"

seq_t a(idx_t n)
{
    /* Convert index to seq_t for arithmetic */
    seq_t N  = (seq_t)n;
    seq_t N2 = N * N;
    seq_t N3 = N2 * N;
    seq_t N4 = N3 * N;
    seq_t N5 = N4 * N;
    seq_t N6 = N5 * N;

    /* Compute the integer numerator of the polynomial */
    seq_t t1 = mul_small(N6, 128);
    seq_t t2 = mul_small(N5, 384);
    seq_t t3 = mul_small(N4, 560);
    seq_t t4 = mul_small(N3, 480);
    seq_t t5 = mul_small(N2, 272);
    seq_t t6 = mul_small(N,  96);
    seq_t t7 = (seq_t)15;

    seq_t numerator = t1 + t2 + t3 + t4 + t5 + t6 + t7;
    return numerator / 15;
}
