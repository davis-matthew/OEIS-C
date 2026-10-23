/*
 * A175441  Denominator of the harmonic mean of the first n positive integers.
 * https://oeis.org/A175441
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, frac
 * Formula:   a(n) = denominator of n / (∑_{k=1..n} 1/k)
 * Range:     u8 n <= 6, u16 n <= 10, u32 n <= 24, u64 n <= 46, u128 n <= 88,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the denominator of the harmonic mean of the first n positive integers.
   Uses the rational representation of the harmonic sum H_n = P/Q, then returns
   the reduced denominator of n/(P/Q) which equals P / gcd(P, n*Q). */
#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    /* a(1) = 1, a(0) is undefined but return 0 for defensive programming */
    if (n == 0)
        return 0;

    /* Start with H_0 = 0/1 */
    seq_t P = 0;   /* numerator of H_n */
    seq_t Q = 1;   /* denominator of H_n */

    for (idx_t k = 1; k <= n; ++k) {
        seq_t k_seq = (seq_t)k;

        /* Reduce fraction before multiplication to keep numbers small */
        seq_t g1 = gcd(k_seq, Q);
        seq_t k_red = k_seq / g1;
        seq_t Q_red = Q / g1;

        /* Compute new numerator and denominator of the sum */
        seq_t new_num = P * k_red + Q_red;
        seq_t new_den = Q_red * k_seq;

        seq_t g2 = gcd(new_num, new_den);
        P = new_num / g2;
        Q = new_den / g2;
    }

    /* Denominator of n / (P/Q) :
       n/(P/Q) = n*Q / P, reduce by gcd(P, n*Q) */
    seq_t n_seq = (seq_t)n;
    seq_t nQ = n_seq * Q;
    seq_t g3 = gcd(P, nQ);
    return P / g3;
}
