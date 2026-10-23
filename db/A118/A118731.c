/*
 * A118731  Numbers k such that 2^k has odd digit sum.
 * https://oeis.org/A118731
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 65, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A118731: numbers k such that 2^k has odd decimal digit sum.
   The implementation increments powers of two in decimal
   representation, checks the parity of their digit sum, and
   records the exponents that satisfy the condition. */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;          /* undefined for n < 1 */

    /* cache of found exponents (k values) */
    static seq_t *k_list = NULL;
    static size_t k_cap = 0;
    static size_t k_len = 0;

    /* decimal digits of the current power of two, least‑significant first */
    static uint8_t *digits = NULL;
    static size_t digits_len = 0;
    static size_t digits_cap = 0;

    static idx_t current_k = 0;   /* exponent of the current power */

    /* initialise static storage once */
    if (k_list == NULL) {
        k_cap = 128;
        k_list = (seq_t *)malloc(k_cap * sizeof(seq_t));
    }
    if (digits == NULL) {
        digits_cap = 128;
        digits = (uint8_t *)malloc(digits_cap * sizeof(uint8_t));
        digits[0] = 1;           /* 2^0 = 1 */
        digits_len = 1;
    }

    /* Generate terms until we have at least n of them */
    while (k_len < n) {
        /* Parity of digit sum: XOR of each digit's parity */
        uint8_t parity = 0;
        for (size_t i = 0; i < digits_len; ++i)
            parity ^= (digits[i] & 1);
        if (parity) {
            /* store current exponent */
            if (k_len >= k_cap) {
                k_cap *= 2;
                k_list = (seq_t *)realloc(k_list, k_cap * sizeof(seq_t));
            }
            k_list[k_len++] = (seq_t)current_k;
        }

        /* Advance to next power: multiply current decimal digits by 2 */
        ++current_k;
        int carry = 0;
        for (size_t i = 0; i < digits_len; ++i) {
            int v = digits[i] * 2 + carry;
            digits[i] = (uint8_t)(v % 10);
            carry = v / 10;
        }
        if (carry) {
            if (digits_len >= digits_cap) {
                digits_cap *= 2;
                digits = (uint8_t *)realloc(digits, digits_cap * sizeof(uint8_t));
            }
            digits[digits_len++] = (uint8_t)carry;
        }
    }

    return k_list[n - 1];
}
