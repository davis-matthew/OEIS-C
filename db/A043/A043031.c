/*
 * A043031  Base-9 palindromes that start with 4.
 * https://oeis.org/A043031
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 101, u32 n <= 911, u64 n <= 911,
 *            u128 n <= 911, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A043031: Base-9 palindromes that start with 4.
   Formula: a(n) = decimal value of the n‑th base‑9 palindrome
   whose most significant digit is 4, listed in increasing order. */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0) {
        return (seq_t)0;  /* undefined for n<1, return 0 */
    }

    /* locate the length of the palindrome and the index within that group */
    idx_t remaining = n;      /* terms still to skip before the desired one */
    idx_t len = 1;            /* current length being considered */
    idx_t count = 1;          /* number of palindromes of length 'len' */

    /* Skip over complete length groups */
    while (remaining > count) {
        remaining -= count;
        ++len;               /* move to next length */

        /* Update the count for the new length:
           for even lengths > 2 the count stays the same as the previous odd length,
           for odd lengths >=3 the count is 9 times the previous count. */
        if (len > 2 && (len & 1)) {
            count *= 9;
        }
    }

    /* remaining now gives the 1‑based index within the current length group */
    idx_t idx = remaining - 1;               /* 0‑based index */
    idx_t free_count = (len - 1) / 2;         /* number of free digits (excluding fixed ends) */

    /* Extract the free digits in base‑9, most‑significant first */
    uint8_t *free_digits = (uint8_t *)malloc(free_count * sizeof(uint8_t));
    idx_t tmp = idx;
    for (idx_t pos = free_count; pos > 0; --pos) {
        free_digits[pos - 1] = (uint8_t)(tmp % 9);
        tmp /= 9;
    }

    /* Build the full sequence of digits of the palindrome */
    uint8_t *digits = (uint8_t *)malloc(len * sizeof(uint8_t));
    digits[0] = 4;
    digits[len - 1] = 4;
    for (idx_t j = 1; j <= free_count; ++j) {
        digits[j] = free_digits[j - 1];
        digits[len - 1 - j] = digits[j];
    }

    /* Convert from base‑9 to decimal value */
    seq_t val = 0;
    for (idx_t i = 0; i < len; ++i) {
        val = val * 9 + (seq_t)digits[i];
    }

    /* Clean up */
    free(free_digits);
    free(digits);

    return val;
}
