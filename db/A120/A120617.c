/*
 * A120617  Hankel transform of g.f. 1/sqrt(1+4*x^2).
 * https://oeis.org/A120617
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(0) = 1, a(1) = -2; a(n) = -4*a(n-2) for n ≥ 2
 * Range:     i8 n <= 6, i16 n <= 14, i32 n <= 30, i64 n <= 33, i128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    /* Compute k = floor(n/2) and (-4)^k by exponentiation by squaring */
    idx_t k = n >> 1;            /* k = n/2 */
    seq_t result = 1;            /* (-4)^0 = 1 */
    seq_t base    = (seq_t)-4;   /* base = -4 */
    idx_t e = k;
    while (e > 0) {
        if (e & 1)        result *= base;   /* result *= base if odd */
        base *= base;                     /* base = base^2      */
        e >>= 1;                     /* e /= 2             */
    }

    /* If n is odd, multiply by -2 */
    if (n & 1) {
        result = (seq_t)-2 * result;
    }

    return result;
}
