/*
 * A127112  n! in base 4.
 * https://oeis.org/A127112
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = decimal integer formed by the base‑4 digits of n!
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 9, u64 n <= 14, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* Return a(n) for OEIS A127112. */
seq_t a(idx_t n)
{
    /* Store the base‑4 digits of n! (least significant digit first). */
    size_t cap = 64;
    size_t len = 0;
    uint8_t *digits = (uint8_t *)malloc(cap);
    if (digits == NULL) {
        /* In the unlikely event of allocation failure, return 0. */
        return (seq_t)0;
    }

    digits[0] = 1;   /* 0! = 1 */
    len = 1;

    for (idx_t i = 2; i <= n; ++i) {
        uint64_t carry = 0;
        for (size_t j = 0; j < len; ++j) {
            uint64_t prod = (uint64_t)digits[j] * (uint64_t)i + carry;
            digits[j] = (uint8_t)(prod & 0x3u);   /* prod % 4   */
            carry    = prod >> 2u;                /* prod / 4   */
        }
        while (carry) {
            if (len == cap) {
                cap <<= 1;
                digits = (uint8_t *)realloc(digits, cap);
                if (digits == NULL) {
                    return (seq_t)0;
                }
            }
            digits[len++] = (uint8_t)(carry & 0x3u);
            carry >>= 2u;
        }
    }

    /* Convert the base‑4 digits to a decimal integer in seq_t. */
    seq_t result = 0;
    for (size_t k = 0; k < len; ++k) {
        result = result * 10 + (seq_t)digits[len - 1 - k];
    }

    free(digits);
    return result;
}
