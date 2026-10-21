/*
 * A203568  a(n) = A026837(n) - A026838(n).
 * https://oeis.org/A203568
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   For k>=1, a((3*k^2-k)/2)=1, a((3*k^2+k)/2)=-1, a(0)=0, otherwise
 *            0
 * Range:     i8 n <= 2, i16 n <= 387, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;

    /* d = 1 + 24*n, must fit into seq_t */
    seq_t d = (seq_t)1 + (seq_t)24 * (seq_t)n;
    seq_t s = floor_sqrt(d);          /* floor(sqrt(d)) */
    if (s * s != d)                  /* not a perfect square */
        return 0;

    /* check for n = k(3k-1)/2  ->  k = (1 + s)/6  */
    seq_t k1_num = s + 1;
    if (k1_num % (seq_t)6 == 0) {
        seq_t k1 = k1_num / 6;
        if (k1 >= 1)
            return 1;
    }

    /* check for n = k(3k+1)/2  ->  k = (s - 1)/6  */
    seq_t k2_num = s - 1;
    if (k2_num % (seq_t)6 == 0) {
        seq_t k2 = k2_num / 6;
        if (k2 >= 1)
            return -1;
    }

    return 0;
}
