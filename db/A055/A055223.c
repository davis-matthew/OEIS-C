/*
 * A055223  One-fourth the digital sum of base 5 representations of 2^n.
 * https://oeis.org/A055223
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = digit_sum_base5(2^n) / 4
 * Range:     u8 n <= 83, u16 n <= 83, u32 n <= 83, u64 n <= 83, u128 n <= 83,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    if (n < 2) return 0; /* offset not defined */

    /* static state */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static idx_t current_n = 0;                 /* exponent of current power (2^current_n) */
    static uint8_t *digits = NULL;
    static size_t digits_len = 0, digits_cap = 0;

    /* initialise static data on first call */
    if (cache == NULL) {
        cache_cap = 1;
        cache = (seq_t*)malloc(cache_cap * sizeof(seq_t));
    }
    if (digits == NULL) {
        digits_cap = 16;
        digits = (uint8_t*)malloc(digits_cap * sizeof(uint8_t));
        digits[0] = 1;      /* 2^0 = 1  */
        digits_len = 1;
    }

    /* ensure cache has room for index n */
    if (n >= cache_cap) {
        size_t new_cap = n + 1;
        seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) exit(1);
        cache = tmp;
        cache_cap = new_cap;
    }

    /* compute powers up to n */
    while (current_n < n) {
        /* multiply current base‑5 digits by 2 */
        uint32_t carry = 0;
        for (size_t i = 0; i < digits_len; ++i) {
            uint32_t val = digits[i] * 2u + carry;
            digits[i] = (uint8_t)(val % 5);
            carry = val / 5;
        }
        while (carry) {
            if (digits_len == digits_cap) {
                digits_cap = digits_cap * 2 + 1;
                uint8_t *tmp = (uint8_t*)realloc(digits, digits_cap * sizeof(uint8_t));
                if (!tmp) exit(1);
                digits = tmp;
            }
            digits[digits_len++] = (uint8_t)(carry % 5);
            carry /= 5;
        }

        /* sum the digits */
        seq_t sum = 0;
        for (size_t i = 0; i < digits_len; ++i) sum += digits[i];

        ++current_n;                 /* now digits represent 2^current_n */
        cache[current_n] = sum / 4;   /* store a(current_n) */
    }

    return cache[n];
}
