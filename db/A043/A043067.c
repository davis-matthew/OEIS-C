/*
 * A043067  a(n)=(s(n)+5)/8, where s(n)=n-th base 8 palindrome that starts
 *          with 3.
 * https://oeis.org/A043067
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = (s(n)+5)/8 where s(n) is the n-th base‑8 palindrome that
 *            starts with 3
 * Range:     u8 n <= 10, u16 n <= 42, u32 n <= 42, u64 n <= 42, u128 n <= 42,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Find the length L of the L‑digit base‑8 palindrome containing the nth element. */
    idx_t cumulative = 0;     /* number of palindromes counted for shorter lengths */
    idx_t len = 1;            /* current palindrome length */

    while (1) {
        idx_t half = (len + 1) / 2;   /* ceil(len/2) */
        /* Count of palindromes of this length: 8^(half‑1) */
        idx_t countLen = 1;
        idx_t k;
        for (k = 1; k < half; ++k) {
            /* protect against overflow: if countLen would exceed n, set it big enough */
            if (countLen > n / 8) { countLen = n + 1; break; }
            countLen *= 8;
        }
        if (cumulative + countLen >= n) break;
        cumulative += countLen;
        ++len;
    }

    /* Position inside this length's block (0‑based). */
    idx_t pos = n - cumulative - 1;

    /* Build the left half digits (including the fixed leading 3). */
    idx_t half = (len + 1) / 2;
    uint8_t halfDigits[128] = {0};  /* maximum needed for very large n */
    halfDigits[0] = 3;
    for (idx_t i = half - 1; i > 0; --i) {   /* fill from middle to second digit */
        halfDigits[i] = pos % 8;
        pos /= 8;
    }

    /* Compute the full palindrome value in base‑10. */
    seq_t value = 0;
    idx_t i;
    for (i = 0; i < len; ++i) {
        seq_t d = (i < half) ? halfDigits[i] : halfDigits[len - 1 - i];
        value = value * 8 + d;
    }

    /* Return (s(n)+5)/8. */
    return (value + 5) / 8;
}
