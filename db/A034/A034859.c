/*
 * A034859  a(n) = C(n+3,4) + 3*C(n+1,3) + 5*C(n-1,2) + 7*n - 15 for n >= 3;
 *          a(1)=1, a(2)=10.
 * https://oeis.org/A034859
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = C(n+3,4) + 3*C(n+1,3) + 5*C(n-1,2) + 7*n - 15
 * Range:     u8 n <= 4, u16 n <= 23, u32 n <= 561, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

seq_t a(idx_t n) {
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)10;

    seq_t n_s = (seq_t)n;
    seq_t t1 = binomial(n + (idx_t)3, (idx_t)4);          /* C(n+3,4) */
    seq_t t2 = binomial(n + (idx_t)1, (idx_t)3);          /* C(n+1,3) */
    seq_t t3 = binomial(n - (idx_t)1, (idx_t)2);          /* C(n-1,2) */

    seq_t res = t1
                + (seq_t)3 * t2
                + (seq_t)5 * t3
                + (seq_t)7 * n_s
                - (seq_t)15;

    return res;
}
