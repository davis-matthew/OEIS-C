/*
 * A190210  a(n) = [n*u + n*v] - [n*u] - [n*v], where u=sqrt(8), v=1/u, and
 *          []=floor.
 * https://oeis.org/A190210
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor(n*u + n*v) - floor(n*u) - floor(n*v) where u =
 *            sqrt(8), v = 1/u, [] = floor
 * Range:     u8 n <= 1, u16 n <= 9, u32 n <= 7463, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   sqrt_floor
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/sqrt_floor.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;            /* undefined below offset, but safe */

    seq_t nn = (seq_t)n * (seq_t)n;   /* n^2          */
    seq_t m  = nn * (seq_t)8;        /* 8*n^2        */
    seq_t k  = sqrt_floor(m);        /* floor(sqrt(8*n^2)) = floor(n*u) */

    /* floor(n*v) = floor(k/8) and k % 8 is used below */
    seq_t q  = (k * (seq_t)9) / (seq_t)8;          /* floor(9*k/8)      */
    seq_t L  = (q + (seq_t)1) * (seq_t)8;          /* 8*(q+1)           */
    seq_t left  = L * L;                           /* (8*(q+1))^2       */
    seq_t right = (seq_t)648 * nn;                 /* 648*n^2 = 81*8*n^2*/

    return (left <= right) ? (seq_t)1 : (seq_t)0;
}
