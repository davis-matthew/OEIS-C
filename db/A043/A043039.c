/*
 * A043039  Palindromes that start with 4.
 * https://oeis.org/A043039
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 122, u32 n <= 2222, u64 n <= 2222,
 *            u128 n <= 2222, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Generates the n-th palindrome in decimal that starts with digit 4.
   The sequence is ordered first by length and then by lexical order
   of the interior digits. This follows OEIS A043039. */
#include <stddef.h>

seq_t a(idx_t n)
{
    /* Special case: the very first palindrome is 4. */
    if (n == 1)
        return (seq_t)4;

    idx_t cumulative = 0;
    size_t L = 1;
    size_t freeDigits;
    idx_t count;

    /* Find the length L that contains the n-th palindrome. */
    while (1) {
        freeDigits = (L - 1) / 2;
        /* count = 10^freeDigits */
        count = 1;
        for (size_t i = 0; i < freeDigits; ++i)
            count *= 10;

        if (cumulative + count >= n)
            break;
        cumulative += count;
        ++L;
    }

    /* Determine the offset inside this length (0‑based). */
    idx_t offset = n - cumulative - 1;
    freeDigits = (L - 1) / 2;

    /* Extract the free digits from offset, most significant first. */
    uint8_t left[64]; /* 64 is ample for any length we handle. */
    idx_t t = offset;
    for (size_t i = 0; i < freeDigits; ++i) {
        left[freeDigits - 1 - i] = t % 10;
        t /= 10;
    }

    /* Construct the palindrome numerically. */
    seq_t result = 0;
    result = result * 10 + 4;          /* first digit */

    /* Add the left side (including the middle digit if odd length). */
    for (size_t i = 0; i < freeDigits; ++i)
        result = result * 10 + left[i];

    /* Mirror the left side, excluding the middle digit for odd lengths. */
    if (L % 2 == 1) {
        if (freeDigits >= 2) {
            for (size_t i = freeDigits - 2; ; --i) {
                result = result * 10 + left[i];
                if (i == 0) break;
            }
        }
    } else {
        for (size_t i = freeDigits; i > 0; --i)
            result = result * 10 + left[i - 1];
    }

    /* last digit */
    result = result * 10 + 4;

    return result;
}
