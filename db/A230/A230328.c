/*
 * A230328  Denominator of n(n+3)/(4(n+1)(n+2)) = Sum_{k = 1..n}
 *          1/(k(k+1)(k+2)).
 * https://oeis.org/A230328
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   a(n) = 4*(n+1)*(n+2)/gcd(4*(n+1)*(n+2), n*(n+3))
 * Range:     u8 n <= 6, u16 n <= 136, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Denominator of n(n+3)/(4(n+1)(n+2)) = Sum_{k=1..n} 1/(k(k+1)(k+2)) */

#include "../utils/gcd.h"

seq_t a(idx_t n) {
    seq_t sn = (seq_t)n;
    seq_t t1 = sn + 1;
    seq_t t2 = sn + 2;

    /* numerator of the fraction */
    seq_t num = 4 * t1 * t2;        /* 4*(n+1)*(n+2) */

    /* denominator of the fraction */
    seq_t den = sn * (sn + 3);      /* n*(n+3) */

    seq_t g = gcd(num, den);
    return num / g;
}
