/*
 * A190081  n + [n*r/t] + [n*s/t]; r=1, s=cos(Pi/5), t=sec(Pi/5).
 * https://oeis.org/A190081
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n + [n*cos(pi/5)] + [n*(cos(pi/5))^2]
 * Range:     u8 n <= 3, u16 n <= 17, u32 n <= 9366, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   isqrt_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/isqrt_seq.h"

seq_t a(idx_t n)
{
    seq_t N = (seq_t)n;
    /* compute floor(n*sqrt5) as floor(sqrt(5*n*n)) */
    seq_t t = isqrt_seq((seq_t)5 * N * N);

    /* floor(n*cos(pi/5)) = floor((N + t)/4)   */
    seq_t first  = (N + t) / 4;
    /* floor(n*cos^2(pi/5)) = floor((3*N + t)/8) */
    seq_t second = ((N * 3) + t) / 8;

    return N + first + second;
}
