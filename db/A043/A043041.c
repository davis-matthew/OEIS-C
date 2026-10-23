/*
 * A043041  Base-10 palindromes that start with 6.
 * https://oeis.org/A043041
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = nth base‑10 palindrome whose first digit is 6
 * Range:     u8 n <= 2, u16 n <= 77, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   pow10, reverse
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow10.h"
#include "../utils/reverse.h"

/* Return the n‑th palindromic number (in decimal) that starts with the digit 6. */
seq_t a(idx_t n)
{
    idx_t remaining = n;

    for (idx_t len = 1; ; ++len) {
        idx_t count;
        if (len == 1) {
            count = 1;
        } else if (len % 2 == 0) {
            count = (idx_t)pow10((len / 2) - 1);   /* 10^((len/2)-1) */
        } else {
            count = (idx_t)pow10((len - 1) / 2);   /* 10^((len-1)/2)   */
        }

        if (remaining <= count) {
            idx_t offset   = remaining - 1;                /* zero‑based index within this block */
            idx_t half_len = (len + 1) / 2;                /* length of the first half        */
            seq_t pow_half = pow10(half_len - 1);          /* 10^(half_len-1)                 */
            seq_t first_half = 6 * pow_half + (seq_t)offset;

            seq_t palindrome;
            if (len % 2 == 0) {                          /* even length */
                palindrome = first_half * pow10(half_len) + reverse(first_half);
            } else {                                    /* odd length */
                palindrome = first_half * pow10(half_len - 1)
                            + reverse(first_half / 10);
            }
            return palindrome;
        } else {
            remaining -= count;      /* skip this block */
        }
    }
}
