/*
 * A094986  a(n) = floor(9^n/5^n).
 * https://oeis.org/A094986
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ⌊9ⁿ / 5ⁿ⌋ (OEIS A094986)
 * Range:     u8 n <= 9, u16 n <= 18, u32 n <= 37, u64 n <= 75, u128 n <= 150,
 *            big any n
 * Helpers:   base5_digits_to_seq_t
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/base5_digits_to_seq_t.h"

/* Returns floor(9ⁿ / 5ⁿ).  The value is computed by keeping 9ⁿ in base‑5
   with the smallest necessary number of digits, then discarding the
   lowest n digits (equivalent to integer division by 5ⁿ). */
seq_t a(idx_t n)
{
    /* Rough estimate of how many base‑5 digits 9ⁿ needs.  log₅(9) ≈ 1.3652. */
    size_t cap = (size_t)((double)n * 1.366 + 3);
    if (cap == 0) cap = 4;           /* ensures room for the n = 0 case */

    uint8_t *digits = (uint8_t *)malloc(cap);
    if (!digits) return (seq_t)0;     /* allocation failure → 0 */

    size_t len = 1;
    digits[0] = 1;                    /* 9⁰ = 1 */

    for (idx_t i = 0; i < n; ++i) {
        /* Multiply the current number by 9 in base‑5. */
        uint32_t carry = 0;
        for (size_t j = 0; j < len; ++j) {
            uint32_t prod = digits[j] * 9u + carry;
            digits[j] = prod % 5u;
            carry    = prod / 5u;
        }
        while (carry) {
            if (len >= cap) {
                cap = cap ? cap * 2 : 4;
                uint8_t *tmp = (uint8_t *)realloc(digits, cap);
                if (!tmp) { free(digits); return (seq_t)0; }
                digits = tmp;
            }
            digits[len++] = carry % 5u;
            carry /= 5u;
        }
    }

    /* Drop the lowest n base‑5 digits (division by 5ⁿ). */
    if ((size_t)n >= len) {
        free(digits);
        return (seq_t)0;
    }
    size_t start = (size_t)n;
    seq_t result = base5_digits_to_seq_t(digits, start, len);

    free(digits);
    return result;
}
