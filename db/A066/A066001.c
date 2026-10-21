/*
 * A066001  Sum of digits of 5^n.
 * https://oeis.org/A066001
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = digit_sum(5^n) (OEIS A066001)
 * Range:     u8 n <= 74, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   ensure_digits, ensure_results
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/ensure_digits.h"
#include "../utils/ensure_results.h"

seq_t a(idx_t n)
{
    /* static state */
    static seq_t *values = NULL;      /* results[0..max_computed] */
    static size_t values_cap = 0;     /* allocated size of values */
    static idx_t max_computed = (idx_t)-1;  /* no values computed yet */

    static uint8_t *digits = NULL;    /* least‑significant digit first */
    static size_t digits_len = 0;     /* number of digits in current 5^k */
    static size_t digits_cap = 0;     /* allocated size of digits */

    /* --- ensure capacity for results array --- */
    if (max_computed == (idx_t)-1) {
        /* first invocation */
        values_cap = 128;
        values = (seq_t *)malloc(values_cap * sizeof(seq_t));
        if (!values) return (seq_t)0;
        digits_cap = 128;
        digits = (uint8_t *)malloc(digits_cap);
        if (!digits) {
            free(values);
            values = NULL;
            return (seq_t)0;
        }
        digits[0] = 1;          /* 5^0 = 1 */
        digits_len = 1;
        values[0] = (seq_t)1;
        max_computed = 0;
    }

    ensure_results((size_t)n + 1, &values, &values_cap);
    if (values == NULL) return (seq_t)0;   /* allocation failed */

    /* --- compute up to requested n --- */
    for (idx_t k = max_computed + 1; k <= n; ++k) {
        /* multiply current number by 5 */
        size_t i;
        uint16_t carry = 0;
        for (i = 0; i < digits_len; ++i) {
            uint16_t prod = (uint16_t)digits[i] * 5u + carry;
            digits[i] = (uint8_t)(prod % 10u);
            carry = prod / 10u;
        }
        while (carry) {
            ensure_digits(digits_len + 1, &digits, &digits_cap);
            if (digits == NULL) return (seq_t)0;
            digits[digits_len++] = (uint8_t)(carry % 10u);
            carry /= 10u;
        }

        /* compute digit sum */
        seq_t sum = 0;
        for (i = 0; i < digits_len; ++i) sum += (seq_t)digits[i];
        values[k] = sum;
    }
    if (max_computed < n) max_computed = n;
    return values[n];
}
