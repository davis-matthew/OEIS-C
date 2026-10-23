/*
 * A179963  Number of permutations p of [2*n+5] such that |p(i)-p(i-1)| > n
 *          for i>1.
 * https://oeis.org/A179963
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ((182*n+2633)/27)*6^n - (3/4)*4^n (for n > 1); initial
 *            values: a(0) = 120, a(1) = 646
 * Range:     u8 n <= 0, u16 n <= 3, u32 n <= 9, u64 n <= 21, u128 n <= 46,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)120;
    if (n == 1) return (seq_t)646;

    /* compute 6^n and 4^(n-1) */
    seq_t pow6 = big_pow((seq_t)6, (seq_t)n);
    seq_t pow4 = big_pow((seq_t)4, (seq_t)(n - 1));

    /* A = 182*n + 2633 */
    seq_t A = (seq_t)182 * n + (seq_t)2633;

    /* split A into q*27 + r and use it safely */
    seq_t q = A / (seq_t)27;
    seq_t r = A % (seq_t)27;

    /* termA = (q*6^n) + (r*6^n)/27 */
    seq_t termA = q * pow6 + (r * pow6) / (seq_t)27;

    /* termB = 3*4^(n-1) */
    seq_t termB = (seq_t)3 * pow4;

    return termA - termB;
}
