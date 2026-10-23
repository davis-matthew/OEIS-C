/*
 * A188082  [nr+kr]-[nr]-[kr], where r=sqrt(3), k=1, [ ]=floor.
 * https://oeis.org/A188082
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor((n+1)*sqrt(3)) – floor(n*sqrt(3)) – 1
 * Range:     u8 n <= 3, u16 n <= 72, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;                 /* guard against an invalid index */

    seq_t nn  = (seq_t)n;
    seq_t t1  = (seq_t)3 * nn * nn;       /* 3·n²  */
    seq_t f1  = floor_sqrt(t1);           /* floor(n*sqrt(3)) */

    seq_t nn1 = nn + 1;
    seq_t t2  = (seq_t)3 * nn1 * nn1;     /* 3·(n+1)²  */
    seq_t f2  = floor_sqrt(t2);           /* floor((n+1)*sqrt(3)) */

    seq_t res = f2 - f1 - (seq_t)1;       /* a(n) = f2 – f1 – 1 */
    return res;
}
