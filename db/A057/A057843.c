/*
 * A057843  a(n) = floor(n*tau^2) - 3, where tau = (1+sqrt(5))/2.
 * https://oeis.org/A057843
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = floor(n*tau^2) - 3
 * Range:     i8 n <= 3, i16 n <= 57, i32 n <= 62, i64 n <= 62, i128 n <= 62,
 *            big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  with  tau = (1+sqrt(5))/2   */
#include <stdint.h>
#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    if (n == 0) {            /* behaviour for offset < 1 is irrelevant */
        return 0;
    }

    seq_t nn = (seq_t)n;                 /* cast index to seq_t */

    /* floor(n*sqrt(5)) = floor(sqrt(5*n^2)) */
    seq_t sq = (seq_t)5 * nn * nn;
    seq_t sqrt_floor = floor_sqrt(sq);

    /* floor(n*phi) where phi = (1+sqrt(5))/2 */
    seq_t phi_floor = (nn + sqrt_floor) / 2;

    /* a(n) = floor(n*phi) + n - 3  */
    return phi_floor + nn - (seq_t)3;
}
