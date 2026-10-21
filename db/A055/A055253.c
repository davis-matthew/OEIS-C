/*
 * A055253  Number of even digits in 2^n.
 * https://oeis.org/A055253
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = number of even decimal digits in the decimal
 *            representation of 2^n
 * Range:     u8 n <= 1568, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* static state for memoisation */
    static seq_t *counts = NULL;       /* cached counts up to current_n */
    static size_t counts_len = 0;      /* allocated length of counts array */

    static uint8_t *digits = NULL;     /* base‑10 digits of 2^current_n, LSB first */
    static size_t digits_len = 0;      /* number of digits stored */
    static size_t digits_cap = 0;      /* capacity of digits array */

    static idx_t current_n = 0;        /* highest n for which digits and counts are known */

    /* Allocate / grow counts array if necessary */
    if ((idx_t)counts_len <= n) {
        size_t new_len = (size_t)n + 1;
        if (counts == NULL) {
            counts = (seq_t*)malloc(new_len * sizeof(seq_t));
            if (!counts) return (seq_t)0;
            counts_len = new_len;
        } else {
            seq_t *tmp = (seq_t*)realloc(counts, new_len * sizeof(seq_t));
            if (!tmp) return (seq_t)0;
            counts = tmp;
            counts_len = new_len;
        }
    }

    /* Initialise the state on first call or when n==0 */
    if (digits == NULL && current_n == 0) {
        /* 2^0 = 1 */
        digits_cap = 16;
        digits = (uint8_t*)malloc(digits_cap);
        if (!digits) return (seq_t)0;
        digits_len = 1;
        digits[0] = 1;
        counts[0] = (seq_t)0;         /* "1" has 0 even digits */
    }

    /* Compute up to the requested n */
    while (current_n < n) {
        /* multiply the current power by 2 */
        uint16_t carry = 0;
        for (size_t i = 0; i < digits_len; ++i) {
            uint16_t temp = (uint16_t)digits[i] * 2 + carry;
            digits[i] = (uint8_t)(temp % 10);
            carry     = temp / 10;
        }
        while (carry) {
            if (digits_len == digits_cap) {
                digits_cap *= 2;
                uint8_t *d = (uint8_t*)realloc(digits, digits_cap);
                if (!d) return (seq_t)0;
                digits = d;
            }
            digits[digits_len++] = (uint8_t)(carry % 10);
            carry /= 10;
        }

        /* count even digits in the new value */
        seq_t evens = 0;
        for (size_t i = 0; i < digits_len; ++i) {
            if ((digits[i] & 1) == 0) ++evens;
        }
        ++current_n;
        counts[current_n] = evens;
    }

    return counts[n];
}
