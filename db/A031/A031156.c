/*
 * A031156  a(n) = 2^A031142(n).
 * https://oeis.org/A031156
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = 2^A031142(n)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 13, u128 n <= 19,
 *            big any n
 * Helpers:   pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/pow_uint.h"

seq_t a(idx_t n)
{
    /* static state: list of exponents that give a(n) */
    static idx_t *exponents   = NULL;
    static size_t exponents_len = 0;
    static size_t exponents_cap = 0;

    /* current power of two in decimal digits */
    static idx_t current_k = 0;          /* exponent of current decimal number */
    static int  current_max_cnt = 1;     /* largest count seen */
    static uint8_t *digits = NULL;      /* least‑significant digit first */
    static size_t digits_len = 0;        /* number of used digits */
    static size_t digits_cap = 0;        /* allocated capacity */

    /* one‑time initialization */
    if (digits_cap == 0) {
        digits_cap = 64;
        digits = (uint8_t*)malloc(digits_cap);
        digits[0] = 1;
        digits_len = 1;

        exponents_cap = 64;
        exponents = (idx_t*)malloc(exponents_cap * sizeof(idx_t));
        exponents[0] = 0;     /* 2^0 = 1  */
        exponents_len = 1;
    }

    /* generate additional exponents until we have at least n terms */
    while (exponents_len < (size_t)n) {
        /* advance to next exponent */
        ++current_k;

        /* multiply current decimal number by 2 */
        int carry = 0;
        for (size_t i = 0; i < digits_len; ++i) {
            int t = digits[i] * 2 + carry;   /* t <= 9*2+1 = 19 */
            digits[i] = (uint8_t)(t % 10);
            carry = t / 10;
        }
        if (carry) {
            if (digits_len >= digits_cap) {
                digits_cap <<= 1;
                digits = (uint8_t*)realloc(digits, digits_cap);
            }
            digits[digits_len++] = (uint8_t)carry;
        }

        /* count non‑zero digits until the first zero from the least significant side */
        int cnt = 0;
        size_t i = 0;
        while (i < digits_len && digits[i] != 0) {
            ++cnt;
            ++i;
        }

        /* a new term if the count strictly increases */
        if (cnt > current_max_cnt) {
            current_max_cnt = cnt;
            if (exponents_len >= exponents_cap) {
                exponents_cap <<= 1;
                exponents = (idx_t*)realloc(exponents,
                                            exponents_cap * sizeof(idx_t));
            }
            exponents[exponents_len++] = current_k;
        }
    }

    /* compute a(n) = 2^exponent */
    return pow_uint(2ULL, exponents[n - 1]);
}
