/*
 * A190427  a(n) = [(b*n+c)*r] - b*[n*r] - [c*r], where (r,b,c)=(golden
 *          ratio,2,1) and []=floor.
 * https://oeis.org/A190427
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor((2*n+1)*phi) - 2*floor(n*phi) - 1, where phi =
 *            (1+sqrt(5))/2
 * Range:     u8 n <= 1, u16 n <= 28, u32 n <= 6966, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/floor_sqrt.h"

static seq_t floor_phi(idx_t k)
{
    seq_t kk = (seq_t)k;           /* k as seq_t   */
    seq_t sq = kk * kk;            /* k^2          */
    sq = sq * (seq_t)5;            /* 5*k^2        */
    seq_t sqrtv = floor_sqrt(sq);  /* floor(k*sqrt5) */
    return (kk + sqrtv) >> 1;      /* floor((k + k*sqrt5)/2) */
}

seq_t a(idx_t n)
{
    seq_t fn     = floor_phi(n);              /* floor(n*phi) */
    seq_t f2n1   = floor_phi((idx_t)(2*n + 1));/* floor((2n+1)*phi) */
    return f2n1 - (fn << 1) - 1;
}
