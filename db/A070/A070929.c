/*
 * A070929  Smallest integer >= 0 of the form x^2 - n^3.
 * https://oeis.org/A070929
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = ceil(n^(3/2))^2 - n^3
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 61, u64 n <= 61, u128 n <= 61,
 *            big any n
 * Helpers:   isqrt_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/isqrt_seq.h"

seq_t a(idx_t n)
{
    seq_t tn = (seq_t)n;
    seq_t n2 = tn * tn;
    seq_t n3 = n2 * tn;          /* n³ */

    seq_t sq = isqrt_seq(n3);    /* floor sqrt(n³) */
    seq_t sq_sq = sq * sq;
    if (sq_sq < n3) {            /* need ceil */
        sq = sq + 1;
        sq_sq = sq * sq;
    }
    return sq_sq - n3;
}
