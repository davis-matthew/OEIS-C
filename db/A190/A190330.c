/*
 * A190330  a(n) = n + [n*r/s] + [n*t/s]; r=1, s=sqrt(2), t=1/s.
 * https://oeis.org/A190330
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n + floor(n / sqrt(2)) + floor(n / 2)
 * Range:     u8 n <= 5, u16 n <= 76, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;                     /* cast index to sequence type */
    /* floor(n / sqrt(2)) = floor( n*sqrt(2)/2 )
     * compute floor( sqrt(2*n*n) / 2 ) to avoid floating point */
    seq_t sqrt_val = floor_sqrt(((seq_t)2) * nn * nn);
    seq_t floor_n_sqrt2 = sqrt_val / 2;      /* integer division gives floor */

    return nn + floor_n_sqrt2 + (nn / 2);    /* n + [n/sqrt2] + [n/2] */
}
