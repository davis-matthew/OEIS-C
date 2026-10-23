/*
 * A043028  Base-9 palindromes that start with 1.
 * https://oeis.org/A043028
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 11, u16 n <= 40, u32 n <= 40, u64 n <= 40, u128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Sequence A043028: Base‑9 palindromes that start with 1 (sorted in
 * increasing order).  The function enumerates all such palindromes by
 * length, determines the length L that contains the requested index n,
 * then decodes the remaining offset into the free digits and builds the
 * palindrome in base 9, finally converting it to decimal (seq_t).
 */
#include <stdint.h>

seq_t a(idx_t n)
{
    /* n is 1‑based; if called with n==0 we simply return 0. */
    if (n == 0)
        return 0;

    idx_t remaining = n;
    idx_t L = 1;               /* current length being examined */

    /* Find the length L such that the nth palindrome lies in this block. */
    while (1) {
        idx_t free_digits = (L - 1) / 2;   /* digits that can vary    */
        idx_t count = 1;                  /* number of palindromes of length L */
        for (idx_t i = 0; i < free_digits; ++i)
            count *= 9;

        if (remaining <= count)
            break;        /* nth palindrome is in this block */

        remaining -= count;   /* skip over this block */
        ++L;                  /* next length */
    }

    /* Decode the offset within this block into the free digits. */
    idx_t free_digits = (L - 1) / 2;
    uint8_t left[64] = {0};          /* left part of the palindrome   */
    idx_t tmp = remaining - 1;       /* 0‑based index within the block */

    for (idx_t i = 0; i < free_digits; ++i) {
        left[free_digits - 1 - i] = (uint8_t)(tmp % 9);
        tmp /= 9;
    }

    /* Build the full palindrome in base 9. */
    uint8_t digits[64] = {0};
    digits[0] = 1;
    digits[L - 1] = 1;

    for (idx_t i = 0; i < free_digits; ++i) {
        digits[1 + i] = left[i];                 /* left side (incl. middle if odd) */
        digits[L - 2 - i] = left[i];             /* mirrored right side              */
    }

    /* Convert the base‑9 digits to decimal (seq_t). */
    seq_t value = 0;
    for (idx_t i = 0; i < L; ++i)
        value = value * 9 + digits[i];

    return value;
}
