/*
 * A190693  [(bn+c)r]-b[nr]-[cr], where (r,b,c)=(sqrt(3),4,0) and [ ]=floor.
 * https://oeis.org/A190693
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor(4*n*sqrt(3)) - 4 * floor(n*sqrt(3))
 * Range:     u8 n <= 1, u16 n <= 18, u32 n <= 131, u64 n <= 131,
 *            u128 n <= 131, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    seq_t sn = (seq_t)n;
    seq_t x1 = sn * sn * (seq_t)3;   /* 3*n^2  */
    seq_t x2 = sn * sn * (seq_t)48;  /* 48*n^2 */

    seq_t u1 = floor_sqrt(x1);  /* floor(n*sqrt(3))   */
    seq_t u2 = floor_sqrt(x2);  /* floor(4*n*sqrt(3)) */

    return u2 - (u1 * (seq_t)4);
}
