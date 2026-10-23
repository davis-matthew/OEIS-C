/*
 * A189998  Numerator of h(n+5) - h(n) where h(n) = Sum_{k=1..n} (1/k) are the
 *          Harmonic numbers.
 * https://oeis.org/A189998
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Numerator of h(n+5)-h(n), where h(n) = Sum_{k=1..n} 1/k.
 *            The numerator is the reduced numerator of (5*n^4 + 60*n^3 +
 *            255*n^2 + 450*n + 274)/((n+1)(n+2)(n+3)(n+4)(n+5))
 * Range:     u8 none, u16 n <= 7, u32 n <= 202, u64 n <= 1000, u128 n <= 1000,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/gcd.h"

seq_t a(idx_t n) {
    seq_t N = (seq_t)n;
    /* Compute the polynomial 5*n^4 + 60*n^3 + 255*n^2 + 450*n + 274 */
    seq_t P = 5 * N * N * N * N;
    P += 60 * N * N * N;
    P += 255 * N * N;
    P += 450 * N;
    P += 274;

    /* Denominator factors (n+1) through (n+5) */
    seq_t d1 = N + 1;
    seq_t d2 = N + 2;
    seq_t d3 = N + 3;
    seq_t d4 = N + 4;
    seq_t d5 = N + 5;

    /* Remove common factors one by one */
    seq_t g = gcd(P, d1);
    if (g > 1) P /= g;

    g = gcd(P, d2);
    if (g > 1) P /= g;

    g = gcd(P, d3);
    if (g > 1) P /= g;

    g = gcd(P, d4);
    if (g > 1) P /= g;

    g = gcd(P, d5);
    if (g > 1) P /= g;

    return P;   /* reduced numerator */
}
