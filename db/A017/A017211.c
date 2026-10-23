/*
 * A017211  a(n) = (9*n + 4)^3.
 * https://oeis.org/A017211
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (9*n + 4)^3
 * Range:     u8 n <= 0, u16 n <= 4, u32 n <= 180, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t tn = (seq_t)n;        /* convert index to seq_t type */
    seq_t base = tn * (seq_t)9 + (seq_t)4;   /* 9*n + 4 */
    seq_t sq = base * base;     /* (9*n + 4)^2 */
    return sq * base;           /* (9*n + 4)^3 */
}
