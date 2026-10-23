/*
 * A120865  a(n) is the number k for which there exists a unique pair (j,k) of
 *          positive integers such that (j + k + 1)^2 - 4*k = 12*n^2.
 * https://oeis.org/A120865
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = -3*n^2 + floor(1 + n*sqrt(3))^2
 * Range:     u8 n <= 4, u16 n <= 68, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/floor_sqrt.h"   /* provides seq_t floor_sqrt(seq_t x); */

seq_t a(idx_t n)
{
    /* cast the index to the sequence type so all arithmetic stays in seq_t */
    seq_t nn   = (seq_t)n;          /* n */
    seq_t n2   = nn * nn;           /* n^2 */
    seq_t t3n2 = (seq_t)3 * n2;     /* 3*n^2 */

    /* floor(n*sqrt(3)) == floor(sqrt(3*n^2)) */
    seq_t t = floor_sqrt(t3n2) + (seq_t)1;   /* floor(n*sqrt(3)) + 1 */

    /* a(n) = -3*n^2 + (floor(n*sqrt(3)) + 1)^2 */
    seq_t res = t * t - t3n2;
    return res;
}
