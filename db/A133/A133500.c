/*
 * A133500  The powertrain or power train map: Powertrain(n): if abcd... is
 *          the decimal expansion of a number n, then the powertrain of n is
 *          the number n' = a^b*c^d* ..., which ends in an exponent or a base
 *          according as the number of digits is even or odd. a(0) = 0 by
 *          convention.
 * https://oeis.org/A133500
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, look
 * Formula:   a(n) is the "powertrain" of n as defined in A133500. For the
 *            decimal expansion d_k … d_2 d_1 d_0 of n, a(n) = Π_{i even}
 *            (d_i)^{d_{i+1}} * (optional trailing base if odd number of
 *            digits). 0^0 is taken as 1. Single‑digit n is just that digit
 * Range:     u8 n <= 27, u16 n <= 47, u32 n <= 2498, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/big_pow.h"

static const int MAX_DIGITS = 20;   /* 2^64 < 10^20 */

seq_t a(idx_t n)
{
    /* Special case for 0.  The decimal representation of 0 has a single
       digit 0 which is a base without exponent, giving the value 0. */
    if (n == 0)
        return 0;

    /* Extract decimal digits into a temporary array in reverse order. */
    uint8_t rev[MAX_DIGITS];
    idx_t d = 0;
    idx_t tmp = n;
    while (tmp > 0) {
        rev[d++] = (uint8_t)(tmp % 10);
        tmp /= 10;
    }

    /* Prepend the most‑significant digit. */
    seq_t result = 1;
    for (idx_t i = 0; i + 1 < d; i += 2) {
        seq_t base = (seq_t)rev[d - 1 - i];
        seq_t exp  = (seq_t)rev[d - 2 - i];
        result *= big_pow(base, exp);
    }

    /* If the number of digits is odd, there is a trailing base digit
       without an exponent; multiply by it directly. */
    if (d % 2 == 1) {
        seq_t last_digit = (seq_t)rev[0];
        result *= last_digit;
    }

    return result;
}
