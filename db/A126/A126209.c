/*
 * A126209  Number of 7's in decimal expansion of 7^n.
 * https://oeis.org/A126209
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 100, u16 n <= 100, u32 n <= 100, u64 n <= 100,
 *            u128 n <= 100, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = number of decimal digits equal to 7 in 7^n */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == (idx_t)0)
        return (seq_t)0;

    /* the number of decimal digits of 7^n is about n*log10(7) < n,
       so an array of 3*n+1 bytes is safely large enough */
    size_t max_len = (size_t)n * 3 + 1;
    uint8_t *digits = (uint8_t *)malloc(max_len);
    if (!digits)
        return (seq_t)0;  /* allocation failed – unlikely in contest */

    size_t len = 1;          /* current number of digits, little‑endian */
    digits[0] = 1;           /* 7^0 = 1 */

    for (idx_t exp = (idx_t)1; exp <= n; ++exp) {
        uint16_t carry = 0;
        for (size_t i = (size_t)0; i < len; ++i) {
            uint16_t prod = (uint16_t)digits[i] * (uint16_t)7 + carry;
            digits[i] = (uint8_t)(prod % 10);
            carry = prod / 10;
        }
        while (carry) {
            digits[len++] = (uint8_t)(carry % 10);
            carry /= 10;
        }
    }

    /* count the digits equal to 7 in the final 7^n */
    seq_t cnt = (seq_t)0;
    for (size_t i = (size_t)0; i < len; ++i) {
        if (digits[i] == (uint8_t)7)
            ++cnt;
    }

    free(digits);
    return cnt;
}
