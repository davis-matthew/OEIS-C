/*
 * A190906  a(n) = gcd(n! / floor(n/2)!^2, 3^n).
 * https://oeis.org/A190906
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, look, hear
 * Formula:   a(n) = gcd(n! / floor(n/2)!^2, 3^n)
 * Range:     u8 n <= 728, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   pow3
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow3.h"

seq_t a(idx_t n) {
    /* floor(n/2) */
    idx_t k = n / 2;
    idx_t twok = 2 * k;
    idx_t v3 = 0;    /* exponent of 3 in n! / floor(n/2)!^2 */

    /* exponent of 3 in C(2k,k) = n! / (k!^2) when n is even, or part of it when n is odd */
    idx_t p = 3;
    while (p <= twok) {
        v3 += (twok / p) - (2 * (k / p));
        if (p > twok / 3) break;   /* avoid overflow of p *= 3 */
        p *= 3;
    }

    /* if n is odd, multiply by the extra factor (2k+1) */
    if ((n & 1) != 0) {
        idx_t factor = twok + 1;   /* 2k+1 */
        while (factor % 3 == 0) {
            ++v3;
            factor /= 3;
        }
    }

    /* final exponent is min(v3, n) because we take gcd with 3^n */
    idx_t exp3 = (v3 > n) ? n : v3;
    return pow3(exp3);
}
