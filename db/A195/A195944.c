/*
 * A195944  Numbers k such that 13^k has no zero in its decimal expansion.
 * https://oeis.org/A195944
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 9, u16 n <= 9, u32 n <= 9, u64 n <= 9, u128 n <= 9,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the n‑th exponent k such that 13^k has no zero digit in base‑10. */

#include <stdlib.h>
#include <stdint.h>
#include <string.h>

seq_t a(idx_t n)
{
    /* static buffers for memoisation */
    static idx_t *terms = NULL;
    static size_t terms_cnt = 0;
    static size_t terms_cap = 0;

    static uint8_t *digits = NULL;    /* least‑significant digit first */
    static size_t digits_len = 0;
    static size_t digits_cap = 0;

    static idx_t last_k = 0;          /* exponent of current digits */

    /* initialise on first call */
    if (!terms) {
        terms_cap = 128;
        terms = (idx_t *)malloc(terms_cap * sizeof(idx_t));
        if (!terms) abort();

        digits_cap = 128;
        digits = (uint8_t *)malloc(digits_cap * sizeof(uint8_t));
        if (!digits) abort();
        digits_len = 1;
        digits[0] = 1;      /* 13^0 == 1 */
        last_k = 0;

        /* exponent 0 is always zero‑free */
        terms[0] = 0;
        terms_cnt = 1;
    }

    /* generate terms until we have at least n of them */
    while (terms_cnt < n) {
        /* multiply current power by 13 to get next power */
        uint32_t carry = 0;
        for (size_t i = 0; i < digits_len; ++i) {
            uint32_t tmp = (uint32_t)digits[i] * 13u + carry;
            digits[i] = (uint8_t)(tmp % 10u);
            carry = tmp / 10u;
        }
        while (carry) {
            if (digits_len == digits_cap) {
                digits_cap *= 2;
                uint8_t *tmp = (uint8_t *)realloc(digits, digits_cap);
                if (!tmp) abort();
                digits = tmp;
            }
            digits[digits_len++] = (uint8_t)(carry % 10u);
            carry /= 10u;
        }

        ++last_k;

        /* check for zero digit */
        int has_zero = 0;
        for (size_t i = 0; i < digits_len; ++i)
            if (digits[i] == 0) { has_zero = 1; break; }

        if (!has_zero) {
            /* store exponent */
            if (terms_cnt == terms_cap) {
                terms_cap *= 2;
                idx_t *tmp = (idx_t *)realloc(terms, terms_cap * sizeof(idx_t));
                if (!tmp) abort();
                terms = tmp;
            }
            terms[terms_cnt++] = last_k;
        }
    }

    return (seq_t)terms[n - 1];
}
