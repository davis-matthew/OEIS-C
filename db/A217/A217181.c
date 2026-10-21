/*
 * A217181  a(n) is the number of digits in the decimal representation of the
 *          smallest power of n that contains six consecutive identical
 *          digits.
 * https://oeis.org/A217181
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   has_six_consecutive
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS formula:
 *  a(n) = number of digits in the smallest power n^k that contains
 *  six consecutive identical decimal digits.
 */
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/has_six_consecutive.h"

seq_t a(idx_t n)
{
    if (n < 2) return 0;        /* offset starts at 2 */

    /* dynamic array for decimal digits, least‑significant first */
    size_t cap  = 64;
    uint8_t *digits = (uint8_t *)malloc(cap);
    if (!digits) return 0;      /* allocation failure */

    /* initialise with n (which fits in idx_t) */
    size_t len = 0;
    uint64_t temp = (uint64_t)n;
    do {
        if (len >= cap) {
            cap *= 2;
            uint8_t *tmp = (uint8_t *)realloc(digits, cap);
            if (!tmp) { free(digits); return 0; }
            digits = tmp;
        }
        digits[len++] = (uint8_t)(temp % 10);
        temp /= 10;
    } while (temp);

    /* check the first power */
    if (has_six_consecutive(digits, len)) {
        free(digits);
        return (seq_t)len;
    }

    /* iterate k = 2,3,... until we find a run */
    for (uint64_t k = 2; ; ++k) {
        /* multiply current number by n */
        uint64_t carry = 0;
        for (size_t i = 0; i < len; ++i) {
            uint64_t prod = (uint64_t)digits[i] * (uint64_t)n + carry;
            digits[i] = (uint8_t)(prod % 10);
            carry = prod / 10;
        }
        while (carry) {
            if (len >= cap) {
                cap *= 2;
                uint8_t *tmp = (uint8_t *)realloc(digits, cap);
                if (!tmp) { free(digits); return 0; }
                digits = tmp;
            }
            digits[len++] = (uint8_t)(carry % 10);
            carry /= 10;
        }

        if (has_six_consecutive(digits, len)) {
            free(digits);
            return (seq_t)len;
        }
    }
}
