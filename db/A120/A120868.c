/*
 * A120868  a(n) is the number k for which there exists a unique pair (j,k) of
 *          positive integers such that (j + k + 1)^2 - 4*k = 5*n^2.
 * https://oeis.org/A120868
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ([n*r+1/2] + 1/2)^2 - (5/4)*n^2 for odd n * a(n) = (1 +
 *            [n*r])^2 - (5/4)*n^2 for even n * where r = sqrt(5)/2. * The
 *            implementation uses integer arithmetic only, avoiding floating *
 *            point and relying on floor_sqrt(seq_t x) provided in the utils
 * Range:     u8 n <= 3, u16 n <= 57, u32 n <= 73, u64 n <= 73, u128 n <= 73,
 *            big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/floor_sqrt.h"  /* Provides floor_sqrt(seq_t) */

seq_t a(idx_t n) {
    seq_t nn = (seq_t)n;                     /* Convert index to seq_t */
    seq_t t   = floor_sqrt((seq_t)5 * nn * nn); /* floor(n*sqrt5) */
    seq_t res;

    if (n & 1) { /* n is odd */
        seq_t s = (t + 1) / 2;                 /* round(n*r) */
        /* temp = 4*s^2 + 4*s + 1 - 5*n^2   (always divisible by 4) */
        seq_t temp = 4 * s * s + 4 * s + 1 - 5 * nn * nn;
        res = temp / 4;
    } else { /* n is even */
        seq_t w = t / 2;                       /* floor(n*r) */
        seq_t u = w + 1;                       /* 1 + floor(n*r) */
        /* temp = 4*u^2 - 5*n^2   (always divisible by 4) */
        seq_t temp = 4 * u * u - 5 * nn * nn;
        res = temp / 4;
    }

    return res;
}
