/*
 * A084768  a(n) = P_n(7), where P_n is n-th Legendre polynomial; also, a(n) =
 *          central coefficient of (1 + 7*x + 12*x^2)^n.
 * https://oeis.org/A084768
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 17, u128 n <= 34,
 *            big any n
 * Helpers:   pow4
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A084768.
   Formula: a(n) = Sum_{k=0..n} binom(n,k)^2 * 3^k * 4^(n-k)
   (central coefficient of (1 + 7x + 12x^2)^n) */
#include <stdint.h>
#include "../utils/pow4.h"

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)7;

    seq_t sum = 0;
    seq_t C = 1;          /* binom(n,0) */
    seq_t power3 = 1;     /* 3^0 */
    seq_t power4 = pow4(n); /* 4^n */

    for (idx_t k = 0; k <= n; ++k)
    {
        /* term = binom(n,k)^2 * 3^k * 4^(n-k) */
        seq_t term = C * C;
        term = term * power3;
        term = term * power4;

        sum += term;

        if (k == n) break;           /* avoid excess calculation */
        C = C * (n - k) / (k + 1);   /* next binomial coefficient */
        power3 *= (seq_t)3;
        power4 /= (seq_t)4;
    }
    return sum;
}
