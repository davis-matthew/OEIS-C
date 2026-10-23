/*
 * A175420  Sequence of numbers after 1st step of iteration defined in
 *          A175419.
 * https://oeis.org/A175420
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, look
 * Formula:   a(n) = ((((D_1)^D_2)^D_3 ...)^D_k) where D_i are the decimal
 *            digits of n, read from least significant (D_1) to most
 *            significant (D_k). The first iteration of the exponentiation
 *            defined in A175419 is computed. For example, n = 33 gives digits
 *            {3,3} and a(33) = 3^3 = 27. n = 27 gives digits {7,2} a...
 * Range:     u8 n <= 36, u16 n <= 66, u32 n <= 266, u64 n <= 378,
 *            u128 n <= 597, big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* NOTE: seq_t and idx_t are supplied by the build system. */

#include <stddef.h>
#include <stdint.h>
#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    /* Extract decimal digits of n, least‑significant first. */
    uint8_t digits[20];
    size_t len = 0;

    if (n == 0) {
        digits[0] = 0;
        len = 1;
    } else {
        uint64_t tmp = (uint64_t)n;
        while (tmp > 0) {
            digits[len++] = (uint8_t)(tmp % 10);
            tmp /= 10;
        }
    }

    /* Compute the iterated exponentiation, using the helper
       big_pow which works with arb‑precision seq_t values. */
    seq_t result = (seq_t)digits[0];
    for (size_t i = 1; i < len; ++i) {
        seq_t exp = (seq_t)digits[i];
        result = big_pow(result, exp);
    }

    return result;
}
