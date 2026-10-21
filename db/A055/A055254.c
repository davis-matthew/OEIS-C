/*
 * A055254  Number of odd digits in 2^n.
 * https://oeis.org/A055254
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = number of odd decimal digits in 2^n Computed by
 *            iteratively multiplying the current power of 2 by 2 and counting
 *            odd decimal digits. A cache is maintained so that repeated calls
 *            with the same or larger n are efficient
 * Range:     u8 n <= 1602, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

seq_t a(idx_t n) {
    /* static cache state */
    static idx_t current_n = 0;
    static seq_t *results = NULL;   /* pow2 odd‐digit counts */
    static size_t results_alloc = 0;
    static uint8_t *digits = NULL;   /* little‑endian decimal digits of 2^current_n */
    static size_t digits_len = 0;
    static size_t digits_cap = 0;

    /* first call: initialise cache and 2^0 = 1 */
    if (results == NULL) {
        results_alloc = 1;
        results = (seq_t *)malloc(results_alloc * sizeof(seq_t));
        if (!results) return 0;            /* had better not happen in the tests */

        digits_cap = 64;
        digits = (uint8_t *)malloc(digits_cap);
        if (!digits) return 0;

        digits[0] = 1;                     /* 2^0 = 1 */
        digits_len = 1;
        results[0] = 1;                    /* one odd digit */
        current_n = 0;
    }

    /* if the requested n has already been computed, return it */
    if (n <= current_n)
        return results[n];

    /* ensure the results array is large enough */
    if ((size_t)n + 1 > results_alloc) {
        size_t new_alloc = results_alloc ? results_alloc : 1;
        while (new_alloc <= (size_t)n)
            new_alloc <<= 1;
        seq_t *tmp = (seq_t *)realloc(results, new_alloc * sizeof(seq_t));
        if (!tmp) return 0;
        results = tmp;
        results_alloc = new_alloc;
    }

    /* iterate from current_n+1 up to n, multiplying by 2 each time */
    for (idx_t i = current_n + 1; i <= n; ++i) {
        /* multiply decimal digit array by 2 */
        unsigned carry = 0;
        for (size_t j = 0; j < digits_len; ++j) {
            unsigned t = digits[j] * 2 + carry;
            digits[j] = t % 10;
            carry = t / 10;
        }
        if (carry) {
            if (digits_len == digits_cap) {
                digits_cap = digits_cap ? digits_cap * 2 : 64;
                uint8_t *new_digits = (uint8_t *)realloc(digits, digits_cap);
                if (!new_digits) return 0;
                digits = new_digits;
            }
            digits[digits_len++] = carry;   /* carry is always 1 here */
        }

        /* count odd digits */
        seq_t odd = 0;
        for (size_t j = 0; j < digits_len; ++j)
            odd += (digits[j] & 1);

        results[i] = odd;
    }

    current_n = n;
    return results[n];
}
