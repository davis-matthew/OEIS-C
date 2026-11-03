/*
 * A195180  a(n) = 5*n - floor(3*n*sqrt(2)).
 * https://oeis.org/A195180
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 5*n - floor(3*n*sqrt(2))
 * Range:     u8 n <= 3, u16 n <= 60, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 86 terms through n = 9939 (2026-11-01)
 * Helpers:   isqrt_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/isqrt_seq.h"

seq_t a(idx_t n)
{
    seq_t N = (seq_t)n;           /* convert index to seq_t */
    seq_t val1 = 5 * N;           /* 5*n */
    seq_t sq   = 18 * N * N;      /* (3*n)^2 = 9*n^2, but 18*n^2 gives (3*sqrt(2)*n)^2 = (3*n*sqrt(2))^2 */
    seq_t val2 = isqrt_seq(sq);  /* floor(3*n*sqrt(2)) = floor(sqrt(18*n^2)) */
    return val1 - val2;
}
