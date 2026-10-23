/*
 * A171971  Integer part of the area of an equilateral triangle with side
 *          length n.
 * https://oeis.org/A171971
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor(n^2 * sqrt(3) / 4)
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 31, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   isqrt_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/isqrt_seq.h"

seq_t a(idx_t n)
{
    /* Compute floor(n^2 * sqrt(3) / 4) by integer arithmetic */
    if (n == 0) return 0;            /* not used, but safe */
    seq_t sn = (seq_t)n;             /* n as seq_t */
    seq_t t  = sn * sn;              /* n^2 */
    seq_t M  = t * t;                /* n^4 */
    M *= (seq_t)3;                   /* 3 * n^4 */
    seq_t k  = isqrt_seq(M);         /* floor(sqrt(3 * n^4)) = floor(n^2 * sqrt(3)) */
    seq_t ans = k / ((seq_t)4);      /* integer division gives floor */
    return ans;
}
