/*
 * A233156  Number of 3 X n 0..2 arrays with no element x(i,j) adjacent to
 *          value 2-x(i,j) horizontally or antidiagonally.
 * https://oeis.org/A233156
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 13, u64 n <= 28, u128 n <= 57,
 *            big any n
 * Helpers:   power2x2_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Formula
   a(1)=27, a(2)=96, a(3)=432, a(4)=1944
   a(n) = 5*a(n-1) - 2*a(n-2) for n > 4
   (computed via fast matrix exponentiation)
*/

#include <stddef.h>   /* for size_t */
#include "../utils/power2x2_mul.h"

seq_t a(idx_t n)
{
    /* Base cases */
    if (n == 1) return (seq_t)27;
    if (n == 2) return (seq_t)96;
    if (n == 3) return (seq_t)432;
    if (n == 4) return (seq_t)1944;
    if (n < 1)  return (seq_t)0;   /* safety, not used */

    /* Matrix exponentiation:  [5  -2; 1  0]^(n-4) * [a4; a3] */
    const seq_t M[4] = { (seq_t)5, (seq_t)-2, (seq_t)1, (seq_t)0 };
    idx_t exp = n - 4;
    seq_t R[4] = { (seq_t)1, (seq_t)0, (seq_t)0, (seq_t)1 }; /* Identity matrix */
    seq_t P[4];
    for (int i = 0; i < 4; ++i) P[i] = M[i];

    while (exp > 0) {
        if (exp & 1) {
            /* R = R * P */
            seq_t r00 = R[0]*P[0] + R[1]*P[2];
            seq_t r01 = R[0]*P[1] + R[1]*P[3];
            seq_t r10 = R[2]*P[0] + R[3]*P[2];
            seq_t r11 = R[2]*P[1] + R[3]*P[3];
            R[0] = r00; R[1] = r01; R[2] = r10; R[3] = r11;
        }
        /* P = P * P */
        seq_t p00 = P[0]*P[0] + P[1]*P[2];
        seq_t p01 = P[0]*P[1] + P[1]*P[3];
        seq_t p10 = P[2]*P[0] + P[3]*P[2];
        seq_t p11 = P[2]*P[1] + P[3]*P[3];
        P[0] = p00; P[1] = p01; P[2] = p10; P[3] = p11;
        exp >>= 1;
    }

    /* Multiply resulting matrix R by initial vector [a4; a3] */
    seq_t a4 = (seq_t)1944;
    seq_t a3 = (seq_t)432;
    return R[0]*a4 + R[1]*a3;
}
