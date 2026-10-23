/*
 * A190204  a(n) = [n*u + n*v] - [n*u] - [n*v], where u=sqrt(6), v=1/u, and
 *          []=floor.
 * https://oeis.org/A190204
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = [n*sqrt(6)+n/sqrt(6)] - [n*sqrt(6)] - [n/sqrt(6)]
 * Range:     u8 n <= 2, u16 n <= 44, u32 n <= 9425, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    seq_t sn = (seq_t)n;
    seq_t n2 = sn * sn;                       /* n^2 */
    seq_t floor_nu = floor_sqrt((seq_t)6 * n2);                      /* floor(n*sqrt(6)) */
    seq_t floor_nv = floor_sqrt(n2 / (seq_t)6);                       /* floor(n/sqrt(6)) */
    seq_t seven_n = sn * (seq_t)7;
    seq_t floor_seven = floor_sqrt(seven_n * seven_n / (seq_t)6);     /* floor(7n/sqrt(6)) */
    return floor_seven - floor_nu - floor_nv;                          /* a(n) is 0 or 1 */
}
