/*
 * A172500  Numerator of fraction whose decimal representation has form
 *          0.(n)(n)(n)...with repeating part n.
 * https://oeis.org/A172500
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = numerator of n/(10^digits-1) with
 *            digits=floor(1+log10(n))
 * Range:     u8 n <= 255, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   gcd, pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow10.h"
#include "../utils/gcd.h"

seq_t a(idx_t n) {
    /* Count decimal digits of n */
    idx_t temp = n;
    idx_t digits = 0;
    while (temp > 0) {
        temp /= 10;
        digits++;
    }

    /* Compute denominator 10^digits - 1 */
    seq_t denom = pow10(digits) - (seq_t)1;

    /* Reduce fraction n / denom and return numerator */
    seq_t nn = (seq_t)n;
    seq_t g  = gcd(nn, denom);
    return nn / g;
}
