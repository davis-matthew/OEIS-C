/*
 * A126205  Number of 3's in decimal expansion of 3^n, with n>=0.
 * https://oeis.org/A126205
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 100, u16 n <= 100, u32 n <= 100, u64 n <= 100,
 *            u128 n <= 100, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = number of decimal digit '3' in 3^n */
#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* dynamic array for decimal digits, little‑endian (least significant digit first) */
    size_t cap = 1;             /* current capacity in digits */
    uint8_t *digits = (uint8_t *)malloc(cap * sizeof(uint8_t));
    if (!digits) return (seq_t)0;   /* allocation failed; return 0 as a safe fallback */
    digits[0] = 1;               /* 3^0 = 1 */
    size_t len = 1;              /* current number of digits */

    /* multiply by 3 n times */
    for (idx_t i = 0; i < n; ++i) {
        unsigned carry = 0;
        for (size_t d = 0; d < len; ++d) {
            unsigned prod = digits[d] * 3U + carry;
            digits[d] = prod % 10U;
            carry = prod / 10U;
        }
        while (carry > 0) {
            if (len == cap) {                      /* grow array */
                size_t new_cap = cap + cap / 2 + 1; /* 1.5x growth */
                uint8_t *tmp = (uint8_t *)realloc(digits, new_cap * sizeof(uint8_t));
                if (!tmp) { free(digits); return (seq_t)0; }
                digits = tmp;
                cap = new_cap;
            }
            digits[len++] = carry % 10U;
            carry /= 10U;
        }
    }

    /* count digit '3' */
    seq_t count = 0;
    for (size_t d = 0; d < len; ++d) {
        if (digits[d] == 3U) ++count;
    }

    free(digits);
    return count;
}
