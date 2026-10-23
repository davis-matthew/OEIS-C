/*
 * A043081  a(n)=(s(n)+8)/10, where s(n)=n-th base 10 palindrome that starts
 *          with 2.
 * https://oeis.org/A043081
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 44, u32 n <= 44, u64 n <= 44, u128 n <= 44,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n) = (s(n)+8)/10 where s(n) is the n‑th base‑10 palindrome
   that starts with the digit 2.  The palindromes are listed in
   increasing numeric order. */
#include <stdint.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    /* n is 1‑based; we work with a 1‑based remainder. */
    idx_t remain = n;

    for (int L = 1; ; ++L) {
        /* Number of palindromes of length L that start with 2. */
        int half = (L + 1) / 2;               /* ceil(L/2) */
        idx_t count = 1;
        for (int i = 1; i < half; ++i)       /* 10^(half‑1) */
            count *= 10;

        if (remain <= count) {
            /* Target palindrome is of length L, offset within this
               length is zero‑based. */
            idx_t offset = remain - 1;

            /* Build the left half digits.  digits[0] is always 2. */
            int digits[32] = {0};
            digits[0] = 2;
            for (int i = half - 1; i >= 1; --i) {
                digits[i] = offset % 10;
                offset /= 10;
            }

            /* Construct the full palindrome numerically. */
            seq_t val = 0;
            for (int i = 0; i < half; ++i)
                val = val * 10 + digits[i];

            int mirror_start = (L % 2 == 0) ? (half - 1) : (half - 2);
            for (int i = mirror_start; i >= 0; --i)
                val = val * 10 + digits[i];

            /* Return (s(n)+8)/10. */
            return (val + 8) / 10;
        }

        remain -= count;   /* move to next length */
    }
}
