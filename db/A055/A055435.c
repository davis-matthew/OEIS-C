/*
 * A055435  (1/18)*Difference between concatenation of n and n^2 and
 *          concatenation of n^2 and n.
 * https://oeis.org/A055435
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, sign
 * Formula:   a(n) = (concat(n, n^2) - concat(n^2, n)) / 18
 * Range:     i8 n <= 3, i16 n <= 23, i32 n <= 40, i64 n <= 40, i128 n <= 40,
 *            big any n
 * Helpers:   digits_seq_local
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/digits_seq_local.h"

static seq_t pow10_seq(idx_t e)
{
    seq_t result = 1;
    for (idx_t i = 0; i < e; ++i)
        result *= (seq_t)10;
    return result;
}

seq_t a(idx_t n)
{
    seq_t n_seq = (seq_t)n;
    seq_t sq = n_seq * n_seq;

    int d = digits_seq_local(n_seq);
    int e = digits_seq_local(sq);

    seq_t td = pow10_seq((idx_t)d);
    seq_t te = pow10_seq((idx_t)e);

    seq_t term = n_seq * (te - 1) - sq * (td - 1);
    return term / (seq_t)18;
}
