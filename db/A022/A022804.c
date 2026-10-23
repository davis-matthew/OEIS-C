/*
 * A022804  a(n) = B(n) + c(n) where B(n) is Beatty sequence [ n*sqrt(2) ] and
 *          c is the complement of B.
 * https://oeis.org/A022804
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*(n + floor(n*sqrt(2)))
 * Range:     u8 n <= 5, u16 n <= 57, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* offset is 1 */

    seq_t sn = (seq_t)n;                  /* convert index to seq_t */
    seq_t sq = sn * sn;                   /* n^2   */
    seq_t dbl_sq = sq + sq;               /* 2 * n^2 = (n*sqrt(2))^2   */
    seq_t fl_sqrt = floor_sqrt(dbl_sq);   /* floor(n*sqrt(2))   */
    seq_t res = (sn + fl_sqrt) * (seq_t)2;/* 2*(n + floor(n*sqrt(2))) */
    return res;
}
