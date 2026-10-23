/*
 * A117970  Position of first 0 counting from the least significant digit in
 *          the ternary expansion of 2^n.
 * https://oeis.org/A117970
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the position (1-indexed) of the first 0 digit in the ternary
   expansion of 2^n, counting from the least significant digit.
   Returns 0 if no such digit occurs. */
#include <stdint.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache of already computed values */
    static seq_t *vals = NULL;           /* a(0)...a(max_n)          */
    static idx_t max_n = 0;              /* highest n already computed */
    static uint8_t *digits = NULL;       /* base-3 digits of 2^max_n  */
    static size_t digits_len = 0;        /* number of digits          */

    /* Initialise for n == 0 */
    if (!vals) {
        vals = (seq_t *)malloc(sizeof(seq_t) * 1);
        if (!vals) return 0;            /* allocation failure */
        vals[0] = 0;                    /* a(0) = 0 */
        max_n = 0;
        digits_len = 1;
        digits = (uint8_t *)malloc(sizeof(uint8_t) * 16);
        if (!digits) return 0;          /* allocation failure */
        digits[0] = 1;                  /* 2^0 = 1 */
    }

    /* Extend the cache up to n if necessary */
    if (n > max_n) {
        /* increase storage for vals */
        seq_t *new_vals = (seq_t *)realloc(vals, (size_t)(n + 1) * sizeof(seq_t));
        if (!new_vals) return 0;       /* allocation failure */
        vals = new_vals;

        /* extend digits array if needed */
        size_t digits_cap = digits_len;
        if (digits_cap < 16) digits_cap = 16;
        for (; max_n < n; ++max_n) {
            /* ensure digits array has capacity to hold one more digit */
            if (digits_len >= digits_cap) {
                digits_cap *= 2;
                uint8_t *tmp = (uint8_t *)realloc(digits, digits_cap);
                if (!tmp) return 0;     /* allocation failure */
                digits = tmp;
            }

            /* multiply current 2^max_n by 2 to get 2^(max_n+1) */
            uint8_t carry = 0;
            for (size_t i = 0; i < digits_len; ++i) {
                uint8_t tmp = digits[i] * 2 + carry;
                digits[i] = tmp % 3;
                carry = tmp / 3;
            }
            if (carry) {
                digits[digits_len++] = carry;
            }

            /* find first zero digit from LSB */
            size_t pos = 0;
            while (pos < digits_len && digits[pos] != 0) ++pos;
            vals[max_n + 1] = (pos == digits_len) ? 0 : (seq_t)(pos + 1);
        }
        max_n = n;
    }

    return vals[n];
}
