/*
 * A108174  Partial sums of the positive integers n according to the rule: if
 *          n is square then subtract n, otherwise add n.
 * https://oeis.org/A108174
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign
 * Formula:   a(n) = n*(n+1)/2 - (m*(m+1)*(2*m+1))/3, where m = floor(sqrt(n))
 * Range:     i8 n <= 18, i16 n <= 266, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   int_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/int_sqrt.h"

seq_t a(idx_t n)
{
    idx_t m = int_sqrt(n);
    seq_t sn = (seq_t)n;
    seq_t sm = (seq_t)m;
    seq_t sum_n = sn * (sn + 1) / 2;
    seq_t sum_sq = sm * (sm + 1) * (2 * sm + 1) / 3;
    return sum_n - sum_sq;
}
