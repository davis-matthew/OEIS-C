/*
 * A172268  a(n) = floor(n*(sqrt(7)-sqrt(3))).
 * https://oeis.org/A172268
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor(n*(sqrt(7)-sqrt(3)))
 * Range:     u8 n <= 2, u16 n <= 8, u32 n <= 468, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    /* Convert the index to the arithmetic type */
    seq_t nn = (seq_t)n;

    /* n^2 */
    seq_t n2 = nn * nn;

    /* floor(n*sqrt(7)) and floor(n*sqrt(3)) via integer square root */
    seq_t s7 = floor_sqrt(n2 * (seq_t)7);
    seq_t s3 = floor_sqrt(n2 * (seq_t)3);

    /* Initial candidate for the sequence value */
    seq_t M = s7 - s3;

    /* L = 4*n^2 - M^2  (always positive) */
    seq_t L = (seq_t)4 * n2 - M * M;

    /* left = L^2 */
    seq_t left = L * L;

    /* right = 12*M^2*n^2  */
    seq_t right = ((seq_t)12 * M) * M * n2;

    /* The sequence value equals M or M-1 depending on the inequality */
    if (left < right)
        M = M - (seq_t)1;

    return M;
}
