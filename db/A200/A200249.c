/*
 * A200249  Number of 0..5 arrays x(0..n-1) of n elements with each no smaller
 *          than the sum of its previous elements modulo 6.
 * https://oeis.org/A200249
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3*a(n-1) + 2*a(n-2), a(1)=6, a(2)=21
 * Range:     u8 n <= 3, u16 n <= 8, u32 n <= 17, u64 n <= 34, u128 n <= 69,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Handle the two initial terms directly. */
    if (n == 1) return (seq_t)6;
    if (n == 2) return (seq_t)21;

    /* Compute M^(n-2) where M = [[3,2],[1,0]]. */
    idx_t k = n - 2;
    /* Initialize M. */
    seq_t m00 = (seq_t)3, m01 = (seq_t)2;
    seq_t m10 = (seq_t)1, m11 = (seq_t)0;
    /* Identity matrix for accumulation. */
    seq_t r00 = 1, r01 = 0, r10 = 0, r11 = 1;

    while (k) {
        if (k & 1) {
            /* r = r * M */
            seq_t t00 = r00 * m00 + r01 * m10;
            seq_t t01 = r00 * m01 + r01 * m11;
            seq_t t10 = r10 * m00 + r11 * m10;
            seq_t t11 = r10 * m01 + r11 * m11;
            r00 = t00; r01 = t01; r10 = t10; r11 = t11;
        }
        /* M = M * M */
        seq_t t00 = m00 * m00 + m01 * m10;
        seq_t t01 = m00 * m01 + m01 * m11;
        seq_t t10 = m10 * m00 + m11 * m10;
        seq_t t11 = m10 * m01 + m11 * m11;
        m00 = t00; m01 = t01; m10 = t10; m11 = t11;
        k >>= 1;
    }

    /* Multiply the resulting matrix by the vector [a2, a1]^T. */
    seq_t a2 = (seq_t)21, a1 = (seq_t)6;
    seq_t result = r00 * a2 + r01 * a1;
    return result;
}
