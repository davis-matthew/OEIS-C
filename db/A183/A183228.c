/*
 * A183228  a(n) is the base-5 digit sum of 10^n+1.
 * https://oeis.org/A183228
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, base
 * Formula:   a(n) = 1 + sum of base‑5 digits of 2^n
 * Range:     u8 n <= 275, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

seq_t a(idx_t n) {
    /* static state – memoised values and the current 2^k in base 5   */
    static int initialized = 0;
    static seq_t *cache = NULL;             /* a(0)…a(max_n)                */
    static size_t cache_cap = 0;            /* allocated capacity of cache   */
    static uint8_t *digits = NULL;          /* digits of 2^k in base 5      */
    static size_t digits_cap = 0;           /* capacity of digits array     */
    static size_t digits_len = 0;           /* current number of digits     */
    static idx_t max_n = 0;                 /* highest n for which a(n) is cached */

    /* first‑time initialisation */
    if (!initialized) {
        cache_cap = 1;
        cache    = (seq_t *)malloc(cache_cap * sizeof(seq_t));

        digits_cap = 64;                   /* plenty for the first few steps */
        digits = (uint8_t *)malloc(digits_cap * sizeof(uint8_t));
        digits[0] = 1;                     /* 2^0 = 1 */
        digits_len = 1;

        cache[0] = (seq_t)1 + (seq_t)1;    /* sum of digits of 1 is 1 -> +1 = 2 */
        max_n = 0;
        initialized = 1;
    }

    /* grow cache if necessary for the requested n */
    if (n >= cache_cap) {
        size_t new_cap = n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cache = tmp;
        cache_cap = new_cap;
    }

    /* if already computed, return cached value */
    if (n <= max_n)
        return cache[n];

    /* compute values from max_n+1 up to n */
    for (idx_t k = max_n + 1; k <= n; ++k) {
        uint32_t carry = 0;
        uint32_t sum   = 0;

        /* multiply current number by 2 in base 5, accumulating the sum */
        for (size_t i = 0; i < digits_len; ++i) {
            uint32_t tmp = (uint32_t)digits[i] * 2 + carry;
            digits[i] = tmp % 5;
            carry = tmp / 5;
            sum += digits[i];
        }

        /* handle remaining carry, appending new digits as needed */
        while (carry > 0) {
            if (digits_len >= digits_cap) {
                size_t new_cap = digits_cap ? digits_cap * 2 : 64;
                uint8_t *new_digits = (uint8_t *)realloc(digits, new_cap * sizeof(uint8_t));
                digits = new_digits;
                digits_cap = new_cap;
            }
            uint8_t dig = carry % 5;
            digits[digits_len++] = dig;
            sum += dig;
            carry /= 5;
        }

        cache[k] = (seq_t)sum + (seq_t)1;   /* a(k) = 1 + digit sum */
    }

    max_n = n;
    return cache[n];
}
