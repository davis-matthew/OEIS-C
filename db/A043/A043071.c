/*
 * A043071  a(n)=(s(n)+1)/8, where s(n)=n-th base 8 palindrome that starts
 *          with 7.
 * https://oeis.org/A043071
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 42, u32 n <= 42, u64 n <= 42, u128 n <= 42,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A043071: a(n) = (s(n)+1)/8, where s(n) is the n‑th base‑8 palindrome that starts with 7. */

#include <stdint.h>

/* Compute the nth term.  n is 1‑based; 0 is undefined (returns 0). */
seq_t a(idx_t n) {
    if (n == 0) return 0;

    /* Find the length L of the palindrome that contains the n‑th number. */
    idx_t remaining = n;
    idx_t L = 1;
    while (1) {
        idx_t left_len = (L + 1) / 2;     /* number of digits in the left half */
        idx_t free     = left_len > 0 ? left_len - 1 : 0; /* digits after the leading 7 */
        idx_t cnt = 1;
        for (idx_t i = 0; i < free; ++i) cnt *= 8;          /* 8^(free) palindromes of this length */

        if (remaining <= cnt) break;          /* the n‑th palindrome is in this length */
        remaining -= cnt;
        ++L;
    }

    /* remaining now is the 1‑based index within palindromes of length L. */
    idx_t left_len = (L + 1) / 2;
    idx_t free = left_len > 0 ? left_len - 1 : 0;
    idx_t comb = remaining - 1;                 /* 0‑based index for free digits */

    /* Build the left half of the palindrome. */
    uint8_t left[64];                          /* plenty enough for any reasonable L */
    left[0] = 7;                               /* leading digit is always 7 */
    for (idx_t i = 0; i < free; ++i) {
        left[left_len - 1 - i] = (uint8_t)(comb % 8);
        comb /= 8;
    }

    /* Construct the full palindrome in base 8 and convert to seq_t. */
    seq_t val = 0;
    for (idx_t i = 0; i < L; ++i) {
        uint8_t d = (i < left_len) ? left[i] : left[L - 1 - i];
        val = val * 8 + d;
    }

    /* Finally, (s + 1)/8. */
    return (val + 1) / 8;
}
