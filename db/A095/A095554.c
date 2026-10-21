/*
 * A095554  a(n) = 81 written in base n.
 * https://oeis.org/A095554
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 81 written in base n
 * Range:     u8 none, u16 none, u32 none, u64 none, u128 none, big any n
 * Helpers:   digits_of_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/digits_of_uint.h"

seq_t a(idx_t n)
{
    /* Base 1: 81 ones.  Handle it separately. */
    if (n <= 1) {
        seq_t val = 0;
        for (int i = 0; i < 81; ++i) {
            val = val * 10 + 1;
        }
        return val;
    }

    /* Compute the base‑n digits of decimal 81 (least significant first). */
    uint64_t temp = 81;
    uint8_t digits[32];          /* 32 is safely larger than any possible length. */
    int len = 0;

    while (temp > 0) {
        uint64_t d = temp % n;
        temp /= n;
        digits[len++] = (uint8_t)d;
    }

    /* Build the decimal integer from the digits, treating each digit as
       its decimal representation.  For a digit d, append its decimal
       representation (which may be more than one decimal digit). */
    seq_t val = 0;
    for (int i = len - 1; i >= 0; --i) {
        uint8_t d = digits[i];
        int k = digits_of_uint(d);          /* number of decimal digits in d */
        seq_t pow10 = 1;
        for (int j = 0; j < k; ++j) {
            pow10 *= 10;
        }
        val = val * pow10 + d;
    }

    return val;
}
