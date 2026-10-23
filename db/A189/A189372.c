/*
 * A189372  a(n) = n + [n*r/t] + [n*s/t]; r=1, s=sqrt(2), t=sqrt(5).
 * https://oeis.org/A189372
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n + floor(n/√5) + floor(n√2/√5)
 * Range:     u8 n <= 2, u16 n <= 40, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined for offset 1, but safe fallback */

    seq_t nn   = (seq_t)n;
    seq_t sf5  = floor_sqrt((seq_t)5 * nn * nn);   /* floor(n*√5)   */
    seq_t sf10 = floor_sqrt((seq_t)10 * nn * nn);  /* floor(n*√10)  */

    return nn + (sf5 / (seq_t)5) + (sf10 / (seq_t)5);
}
