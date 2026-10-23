/*
 * A029219  Expansion of 1/((1-x^2)*(1-x^6)*(1-x^8)*(1-x^9)).
 * https://oeis.org/A029219
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor((n^3 + 51*n^2 + 760*n + 5440)/5184 - (n^2 + 25*n +
 *            138)*(n mod 2)/192 + n*(((n+2) mod 3) - ((n+1) mod 3))/162)
 * Range:     u8 n <= 93, u16 n <= 681, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    /* use a common denominator 5184  (192*27, 162*32) */
    int64_t N = (int64_t)n;
    int64_t term1 = N*N*N + 51*N*N + 760*N + 5440;
    int64_t mod2 = N & 1;
    int64_t term2 = (N*N + 25*N + 138) * 27 * mod2;
    int64_t diff = ((N + 2) % 3) - ((N + 1) % 3);
    int64_t term3 = N * diff * 32;
    int64_t numer = term1 - term2 + term3;
    return (seq_t)(numer / 5184);
}
