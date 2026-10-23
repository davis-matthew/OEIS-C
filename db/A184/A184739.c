/*
 * A184739  floor(n*s+h-h*s), where s=3+sqrt(5), h=1/2; complement of A184738.
 * https://oeis.org/A184739
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor(n*(3+sqrt(5)) + 1/2 - (1/2)*(3+sqrt(5)))
 * Range:     u8 n <= 2, u16 n <= 29, u32 n <= 120, u64 n <= 120,
 *            u128 n <= 120, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    /* Return 0 for n == 0 (out of formal range). */
    if (n == (idx_t)0)
        return (seq_t)0;

    seq_t n_s = (seq_t)n;
    seq_t m   = (seq_t)2 * n_s - (seq_t)1;           /* m = 2n – 1          */
    seq_t t   = (seq_t)5 * m * m;                   /* t = 5 * m²           */
    seq_t s   = floor_sqrt(t);                      /* floor(sqrt(t)) = floor(m*sqrt(5)) */
    seq_t h   = s / (seq_t)2;                       /* floor(m*sqrt(5)/2)   */
    seq_t res = (seq_t)3 * n_s - (seq_t)1 + h;      /* 3n – 1 + floor(m*sqrt(5)/2) */
    return res;
}
