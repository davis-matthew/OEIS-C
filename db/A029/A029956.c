/*
 * A029956  Numbers that are palindromic in base 11.
 * https://oeis.org/A029956
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 34, u16 n <= 662, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A029956: a(n) = the nth non‑negative integer whose base‑11 representation is a palindrome. */

#include <stdint.h>

/* Compute the nth palindrome in base 11.  The sequence is indexed starting at 1 (a(1)=0). */
seq_t a(idx_t n)
{
    /* Single‑digit palindromes: 0,1,2,…,10. */
    if (n <= 11) {
        return (seq_t)(n - 1);
    }

    /* Find the length L of the palindrome that contains the n‑th term.
       cumulative counts the number of palindromes with length < L. */
    idx_t cumulative = 11;          /* count for length 1 */
    idx_t len = 2;

    while (1) {
        idx_t count;
        if (len & 1U) {            /* odd length: L = 2k+1 */
            idx_t k = (len - 1) / 2;
            idx_t pow = 1;
            for (idx_t i = 0; i < k; ++i) {
                pow *= 11;
            }
            count = 10 * pow;     /* 10 choices for leading digit (1–10) */
            if (cumulative + count >= n) {
                break;
            }
            cumulative += count;
            ++len;
        } else {                   /* even length: L = 2k */
            idx_t k = len / 2;
            idx_t pow = 1;
            if (k > 1) {
                for (idx_t i = 0; i < k - 1; ++i) {
                    pow *= 11;
                }
            }
            count = 10 * pow;     /* 10 choices for leading digit (1–10) */
            if (cumulative + count >= n) {
                break;
            }
            cumulative += count;
            ++len;
        }
    }

    /* Index of the desired palindrome among those of length len (0‑based). */
    idx_t offset = n - cumulative - 1;

    /* Build the base‑11 digits of the palindrome. */
    uint8_t digits[64] = {0};        /* len will not exceed 20 for n up to 10000 */

    if (len & 1U) {                  /* odd length: L = 2k+1 */
        idx_t k = (len - 1) / 2;
        idx_t pow = 1;
        for (idx_t i = 0; i < k; ++i) {
            pow *= 11;
        }
        idx_t leading_idx = offset / pow;          /* 0 … 9 */
        digits[0] = (uint8_t)(leading_idx + 1);    /* leading digit cannot be 0 */

        idx_t rem = offset % pow;

        /* Extract the k remaining digits (most significant first). */
        for (idx_t j = 0; j < k; ++j) {
            digits[1 + (k - 1 - j)] = (uint8_t)(rem % 11);
            rem /= 11;
        }

        /* Mirror to the right half. */
        for (idx_t i = 0; i < k; ++i) {
            digits[len - 1 - i] = digits[i];
        }
    } else {                          /* even length: L = 2k */
        idx_t k = len / 2;
        idx_t pow = 1;
        if (k > 1) {
            for (idx_t i = 0; i < k - 1; ++i) {
                pow *= 11;
            }
        }
        idx_t leading_idx = offset / pow;          /* 0 … 9 */
        digits[0] = (uint8_t)(leading_idx + 1);    /* leading digit cannot be 0 */

        idx_t rem = offset % pow;

        /* Extract the k-1 remaining digits (most significant first). */
        for (idx_t j = 0; j < k - 1; ++j) {
            digits[1 + (k - 2 - j)] = (uint8_t)(rem % 11);
            rem /= 11;
        }

        /* Mirror to the right half. */
        for (idx_t i = 0; i < k; ++i) {
            digits[len - 1 - i] = digits[i];
        }
    }

    /* Convert the base‑11 digits to the final value. */
    seq_t result = 0;
    for (idx_t i = 0; i < len; ++i) {
        result = result * 11 + digits[i];
    }

    return result;
}
