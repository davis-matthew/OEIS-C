/*
 * A089662  a(n) = S1(n,5), where S1(n,t) = Sum_{k=0..n} k^t * Sum_{j=0..k}
 *          binomial(n,j).
 * https://oeis.org/A089662
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 0 (n=0) a(n) = 2^(n-7)*n*(21*n^5 + 61*n^4 + 55*n^3 +
 *            15*n^2 - 28*n + 4) for n>=1 (precomputed values for n<7 are used
 *            to avoid negative powers of 2)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 12, u64 n <= 35, u128 n <= 91,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /* precomputed terms for n = 0 .. 6 */
    static const seq_t small[7] = {
        (seq_t)0,
        (seq_t)2,
        (seq_t)131,
        (seq_t)2172,
        (seq_t)20386,
        (seq_t)138580,
        (seq_t)763824
    };

    if (n <= 6)
        return small[n];

    /* n >= 7: use the explicit formula */
    seq_t n_seq = (seq_t)n;

    /* compute n^2 .. n^5 */
    seq_t n2 = n_seq * n_seq;          /* n^2 */
    seq_t n3 = n2 * n_seq;             /* n^3 */
    seq_t n4 = n3 * n_seq;             /* n^4 */
    seq_t n5 = n4 * n_seq;             /* n^5 */

    /* polynomial part: 21*n^5 + 61*n^4 + 55*n^3 + 15*n^2 - 28*n + 4 */
    seq_t poly = 21 * n5 + 61 * n4 + 55 * n3 + 15 * n2 - 28 * n_seq + 4;

    /* multiply by n */
    poly *= n_seq;

    /* multiply by 2^(n-7) */
    seq_t pow = pow2(n - 7);           /* n-7 >= 0 as n >= 7 */
    return poly * pow;
}
