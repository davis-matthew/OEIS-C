/*
 * A175456  (Digit sum of 5^n) mod n.
 * https://oeis.org/A175456
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = (sum of decimal digits of 5^n) mod n
 * Range:     u8 n <= 100, u16 n <= 100, u32 n <= 100, u64 n <= 100,
 *            u128 n <= 100, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

/* Compute a(n) = digit sum of 5^n mod n, with n >= 1. */
seq_t a(idx_t n)
{
    if (n == 0)   /* safety guard – not part of the sequence */
        return 0;

    /* Estimate decimal digit count: floor(n*log10(5)) + 2 for safety */
    idx_t approx_digits = (idx_t)((double)n * 0.69897) + 2;
    size_t capacity = (size_t)(approx_digits > 128 ? approx_digits : 128);
    size_t len = 1;                         /* current number of digits (LSB first) */
    uint8_t *digits = (uint8_t *)malloc(capacity * sizeof(uint8_t));
    if (!digits) return 0;                  /* allocation failure – unlikely here */

    digits[0] = 1;                          /* start with 5^0 = 1 */

    for (idx_t i = 0; i < n; ++i) {
        uint32_t carry = 0;
        for (size_t j = 0; j < len; ++j) {
            uint32_t prod = (uint32_t)digits[j] * 5u + carry;
            digits[j] = (uint8_t)(prod % 10u);
            carry = prod / 10u;
        }
        while (carry) {
            if (len == capacity) {
                capacity <<= 1;
                uint8_t *tmp = (uint8_t *)realloc(digits, capacity * sizeof(uint8_t));
                if (!tmp) { free(digits); return 0; }
                digits = tmp;
            }
            digits[len++] = (uint8_t)(carry % 10u);
            carry /= 10u;
        }
    }

    seq_t sum = 0;
    seq_t mod = (seq_t)n;
    for (size_t j = 0; j < len; ++j) {
        sum += digits[j];
        if (sum >= mod) sum %= mod;        /* keep sum small */
    }

    free(digits);
    return sum % mod;
}
