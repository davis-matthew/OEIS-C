/*
 * A043080  a(n) = (s(n) + 9)/10, where s(n) is the n-th base 10 palindrome
 *          that starts with 1.
 * https://oeis.org/A043080
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = (s(n)+9)/10 where s(n) is the n‑th base‑10 palindrome
 *            starting with 1
 * Range:     u8 n <= 12, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;           /* undefined for 0, return 0 */

    /* Find the length L of the palindrome that contains the n-th element. */
    idx_t remaining = n;
    idx_t L = 1;

    while (1) {
        /* number of palindromes of length L: 10^floor((L-1)/2) */
        idx_t k = (L - 1) / 2;
        idx_t count = 1;
        for (idx_t i = 0; i < k; ++i) count *= 10;

        if (remaining <= count) break;   /* n falls in this block */
        remaining -= count;
        ++L;
    }

    /* offset within the current block (0‑based) */
    idx_t offset = remaining - 1;
    idx_t k = (L - 1) / 2;              /* number of free digits */
    unsigned char free_digits[64] = {0}; /* enough for all reasonable n */

    /* Decode offset into base‑10 digits, most significant first. */
    for (idx_t i = k; i > 0; --i) {
        free_digits[i - 1] = offset % 10;
        offset /= 10;
    }

    /* Build the full palindrome s(n). */
    unsigned char pal[128] = {0};       /* length L ≤ 127 for all n tested */
    pal[0] = 1;
    pal[L - 1] = 1;
    for (idx_t i = 0; i < k; ++i) {
        pal[1 + i] = free_digits[i];
        pal[L - 2 - i] = free_digits[i];
    }

    /* Convert digits to a seq_t number. */
    seq_t s = 0;
    for (idx_t i = 0; i < L; ++i) {
        s = s * 10 + pal[i];
    }

    /* a(n) = (s + 9)/10  ==  s/10 + 1  (integer division) */
    return s / 10 + 1;
}
