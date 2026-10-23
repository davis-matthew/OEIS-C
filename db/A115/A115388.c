/*
 * A115388  Numerator of rational part of raw moment n of the line point
 *          picking problem.
 * https://oeis.org/A115388
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, frac, easy
 * Formula:   a(n) = numerator of 1 + (2*n)*Sum_{k=1..n} (-1)^(n+k+1)/k
 * Range:     i8 n <= 4, i16 n <= 9, i32 n <= 19, i64 n <= 29, i128 n <= 29,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd.h"

seq_t a(idx_t n) {
    if (n < 1) return 0;  /* undefined for n < 1, return 0 */

    seq_t A = 0;          /* numerator of H_alt(n) */
    seq_t B = 1;          /* denominator   of H_alt(n) */

    for (idx_t k = 1; k <= n; ++k) {
        seq_t sign = (k & 1) ? (seq_t)-1 : (seq_t)1;      /* (-1)^k */
        seq_t k2   = (seq_t)k;                            /* cast to seq_t */

        seq_t num = A * k2 + sign * B;                    /* new numerator */
        seq_t den = B * k2;                               /* new denominator */

        seq_t g = gcd(num >= 0 ? num : -num, den);       /* reduce fraction */
        A = num / g;
        B = den / g;
    }

    /* Compute S = 1 + 2*n*(-1)^(n+1)*H_alt(n) */
    seq_t sign2 = (n & 1) ? (seq_t)1 : (seq_t)-1;         /* (-1)^(n+1) */
    seq_t temp2 = (seq_t)(2 * n);                         /* factor 2*n */

    seq_t prod = temp2 * sign2;                           /* 2*n*(-1)^(n+1) */
    prod *= A;                                           
    seq_t N = B + prod;                                   /* numerator of S */

    seq_t g2 = gcd(N >= 0 ? N : -N, B);                  /* reduce to a(n) */
    return N / g2;
}
