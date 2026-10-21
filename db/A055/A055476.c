/*
 * A055476  Powers of ten written in base 5.
 * https://oeis.org/A055476
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(n) = A000866(n) followed by n zeros
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 13, u128 n <= 15,
 *            big any n
 * Helpers:   pow10, seq_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/pow10.h"
#include "../utils/seq_mul.h"

seq_t a(idx_t n)
{
    /* Compute the base-5 digits of 2^n (least‑significant digit first). */
    size_t capacity = (size_t)n + 2;           /* safe upper bound      */
    uint8_t *digits = (uint8_t *)malloc(capacity);
    if (!digits) return (seq_t)0;

    digits[0] = 1;   /* 2^0 */
    size_t len = 1;

    for (idx_t i = 1; i <= n; ++i) {
        uint8_t carry = 0;
        for (size_t j = 0; j < len; ++j) {
            uint8_t prod = digits[j] * 2 + carry;
            digits[j] = prod % 5;
            carry    = prod / 5;
        }
        if (carry) {
            if (len + 1 >= capacity) {         /* resize if necessary */
                capacity = capacity * 2 + 1;
                uint8_t *tmp = (uint8_t *)realloc(digits, capacity);
                if (!tmp) {
                    free(digits);
                    return (seq_t)0;
                }
                digits = tmp;
            }
            digits[len++] = carry;
        }
    }

    /* Convert the base‑5 digits (most significant first) to a decimal integer. */
    seq_t value = 0;
    for (size_t j = 0; j < len; ++j) {
        size_t idx = len - 1 - j;              /* most significant digit */
        value = (seq_t)((value * 10) + digits[idx]);
    }

    /* Append n decimal zeros: multiply by 10^n. */
    seq_t pow10n  = pow10(n);
    seq_t result  = seq_mul(value, pow10n);

    free(digits);
    return result;
}
