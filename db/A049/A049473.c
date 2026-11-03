/*
 * A049473  Nearest integer to n/sqrt(2).
 * https://oeis.org/A049473
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor(n*sqrt(2)) - floor(n/sqrt(2))
 * Range:     u8 n <= 11, u16 n <= 763, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 84 terms through n = 9647 (2026-11-01)
 * Helpers:   isqrt_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/isqrt_seq.h"

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;               /* convert index to seq_t */
    seq_t sq = nn * nn;                /* n^2  */
    sq *= (seq_t)2;                    /* 2*n^2  */
    seq_t t  = isqrt_seq(sq);         /* floor(n*sqrt(2))  */
    seq_t b  = t / (seq_t)2;           /* floor(n/√2) = floor(n*√2/2) */
    return t - b;                      /* desired difference */
}
