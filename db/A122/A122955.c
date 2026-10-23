/*
 * A122955  Numbers k such that although the number of digits in 5^k is
 *          larger, the sum of the digits is less than that of the previous
 *          power of 5.
 * https://oeis.org/A122955
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 60, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Series: numbers k such that the number of digits in 5^k increases
   while the sum of the digits of 5^k is less than that of 5^(k-1). */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

/* seq_t and idx_t are defined by the build environment. */

seq_t a(idx_t n)
{
    /* Static state so the function can be called in any order. */
    static bool initialized = false;
    static idx_t current_k = 1;             /* exponent of the current 5^k */
    static idx_t prev_len = 1;              /* number of digits of 5^(k-1) */
    static uint32_t digit_sum = 5;          /* digit sum of 5^current_k */
    static uint32_t prev_sum = 5;           /* digit sum of previous exponent */

    /* Decimal digits of the current power, least‑significant first. */
    static uint8_t *digits = NULL;
    static size_t digits_len = 0, digits_cap = 0;

    /* Cache of found terms. */
    static idx_t *terms = NULL;
    static size_t terms_len = 0, terms_cap = 0;

    /* ---------- initialisation ---------- */
    if (!initialized) {
        digits_cap = 128;
        digits = (uint8_t *)malloc(digits_cap);
        digits[0] = 5;          /* 5^1 */
        digits_len = 1;

        terms_cap = 128;
        terms = (idx_t *)malloc(terms_cap * sizeof(idx_t));
        terms_len = 0;

        initialized = true;
    }

    if (n == 0)                 /* undefined for n==0, but return 0 safely */
        return 0;

    /* ---------- extend the cache until we have at least n terms ---------- */
    while (terms_len < (size_t)n) {
        /* Compute next power 5^(current_k+1) */
        idx_t new_k = current_k + 1;
        uint32_t carry = 0;

        /* Multiply the current decimal number by 5 in place. */
        for (size_t i = 0; i < digits_len; ++i) {
            uint32_t prod = digits[i] * 5 + carry;    /* prod <= 49 */
            uint8_t newd = prod % 10;
            carry = prod / 10;
            digit_sum += newd - digits[i];           /* update digit sum */
            digits[i] = newd;
        }

        /* Append any remaining carry as new digits. */
        while (carry) {
            uint8_t newd = carry % 10;
            carry /= 10;
            if (digits_len == digits_cap) {
                digits_cap *= 2;
                digits = (uint8_t *)realloc(digits, digits_cap);
            }
            digits[digits_len++] = newd;
            digit_sum += newd;
        }

        /* Check the two conditions for the series. */
        if (digits_len > prev_len && digit_sum < prev_sum) {
            if (terms_len == terms_cap) {
                terms_cap *= 2;
                terms = (idx_t *)realloc(terms, terms_cap * sizeof(idx_t));
            }
            terms[terms_len++] = new_k;   /* record this exponent */
        }

        /* Prepare for next iteration. */
        prev_len = digits_len;
        prev_sum = digit_sum;
        current_k = new_k;
    }

    return (seq_t)terms[n - 1];
}
