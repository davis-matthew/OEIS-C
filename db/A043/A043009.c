/*
 * A043009  Base-5 palindromes that start with 4.
 * https://oeis.org/A043009
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 39, u32 n <= 39, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Base-5 palindromes that start with 4:
   a(n) = nth number whose base‑5 representation is a palindrome
   and begins with the digit 4.  The terms are listed in increasing
   order. */
#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0; /* sequence is 1‑based */

    seq_t cumulative = 0; /* cumulative count up to current length */
    seq_t count;          /* number of palindromes of current length */

    for (idx_t len = 1; ; ++len) {
        idx_t half = (len - 1) / 2;
        count = 1;
        for (idx_t i = 0; i < half; ++i) count *= 5; /* 5^half */

        cumulative += count;
        if (cumulative >= (seq_t)n) {
            seq_t rank = (seq_t)n - (cumulative - count); /* 1‑based rank in this length */
            seq_t r = rank - 1;                           /* 0‑based index */

            /* free digits in base‑5 (most significant first) */
            uint8_t free_digits[64] = {0};
            for (idx_t i = 0; i < half; ++i) {
                free_digits[half - 1 - i] = (uint8_t)(r % 5);
                r /= 5;
            }

            /* assemble full digit array of the palindrome */
            uint8_t digits[64] = {0};
            digits[0] = 4;
            for (idx_t i = 1; i <= half; ++i)
                digits[i] = free_digits[i - 1];
            for (idx_t i = half + 1; i < len; ++i)
                digits[i] = digits[len - 1 - i];

            /* convert from base‑5 to decimal */
            seq_t value = 0;
            for (idx_t i = 0; i < len; ++i)
                value = value * 5 + digits[i];

            return value;
        }
    }
}
