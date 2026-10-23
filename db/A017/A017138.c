/*
 * A017138  a(n) = (8*n+6)^2.
 * https://oeis.org/A017138
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (8*n+6)^2
 * Range:     u8 n <= 1, u16 n <= 31, u32 n <= 8191, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t t = (seq_t)n;   /* cast index to seq_t */
    t = t * 8 + 6;        /* compute (8*n + 6) */
    return t * t;         /* square */
}
