/*
 * A127114  n! in base 7.
 * https://oeis.org/A127114
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = the number formed by writing n! in base 7, then
 *            interpreting that base‑7 string as a decimal integer
 * Range:     u8 n <= 5, u16 n <= 7, u32 n <= 11, u64 n <= 18, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0 || n == 1) {  /* 0! = 1! = 1 */
        return (seq_t)1;
    }

    /* dynamic array of base‑7 digits, least significant digit first */
    size_t digits_cap = 128;
    uint8_t *digits = (uint8_t *)malloc(digits_cap);
    if (!digits) return (seq_t)0;          /* out of memory: no result */
    size_t len = 1;                       /* digits = {1} for 1! */
    digits[0] = 1;

    for (idx_t k = 2; k <= n; ++k) {
        uint64_t carry = 0;
        for (size_t i = 0; i < len; ++i) {
            uint64_t tmp = (uint64_t)digits[i] * (uint64_t)k + carry;
            digits[i] = (uint8_t)(tmp % 7);
            carry     = tmp / 7;
        }
        /* append remaining carry digits */
        while (carry) {
            if (len == digits_cap) {
                digits_cap <<= 1;
                uint8_t *tmp = (uint8_t *)realloc(digits, digits_cap);
                if (!tmp) { free(digits); return (seq_t)0; }
                digits = tmp;
            }
            digits[len++] = (uint8_t)(carry % 7);
            carry /= 7;
        }
    }

    /* convert base‑7 digits to the decimal integer requested by OEIS */
    seq_t result = 0;
    for (size_t i = 0; i < len; ++i) {
        /* process most significant digit first */
        result = result * 10 + digits[len - 1 - i];
    }

    free(digits);
    return result;
}
