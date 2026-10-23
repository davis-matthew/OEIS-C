/*
 * A108167  Partial sums of the positive integers n according to the rule: if
 *          n is square then subtract sqrt(n) else add n.
 * https://oeis.org/A108167
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign
 * Formula:   a(n) = n*(n+1)/2 - m*(m+1)*(m+2)/3, where m = floor(sqrt(n))
 * Range:     i8 n <= 17, i16 n <= 261, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   int_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/int_sqrt.h"   /* provides idx_t int_sqrt(idx_t x) */

seq_t a(idx_t n)
{
    idx_t m = int_sqrt(n);                       /* floor(sqrt(n)) */
    seq_t nseq = (seq_t)n;
    seq_t mseq = (seq_t)m;
    seq_t total = nseq * (nseq + 1) / 2;          /* sum of 0..n */
    seq_t sub   = mseq * (mseq + 1) * (mseq + 2) / 3; /* subtract squares adjustment */
    return total - sub;
}
