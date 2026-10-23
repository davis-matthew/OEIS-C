/*
 * A043078  a(n)=(s(n)+2)/9, where s(n)=n-th base 9 palindrome that starts
 *          with 7.
 * https://oeis.org/A043078
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = (s(n)+2)/9, where s(n) is the n-th base‑9 palindrome that
 *            starts with digit 7
 * Range:     u8 n <= 2, u16 n <= 181, u32 n <= 1600, u64 n <= 1600,
 *            u128 n <= 1600, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* Find length L of the n-th palindrome */
    idx_t remaining = n;          /* number of palindromes we still need to skip */
    size_t L = 1;                 /* current length */

    while (1) {
        size_t m = (L + 1) / 2;   /* number of digits in the left half (including middle if odd) */
        size_t cnt = 1;           /* count of palindromes of length L */
        for (size_t i = 0; i < m - 1; ++i) cnt *= 9;   /* 9 options for each remaining digit */

        if (remaining <= cnt)
            break;
        remaining -= cnt;
        ++L;
    }

    /* Index (0‑based) inside this length block */
    idx_t idx = remaining - 1;
    size_t m = (L + 1) / 2;
    size_t low_len = m - 1;               /* digits after the leading 7 in the left half */
    uint8_t low[20];                      /* enough for any practical L */

    /* Decode idx into base‑9 digits (least significant first) */
    idx_t tmp = idx;
    for (size_t pos = 0; pos < low_len; ++pos) {
        low[pos] = (uint8_t)(tmp % 9);
        tmp /= 9;
    }
    /* Remaining tmp should be zero; the digits are now stored in low[0..low_len-1]
       (least significant first). We need most significant first for construction. */

    /* Build all digits of the palindrome */
    uint8_t digits[64]; /* ample size; max length for 64‑bit n is well below 64 */
    for (size_t i = 0; i < m; ++i) {
        if (i == 0)
            digits[i] = 7;                     /* first digit fixed to 7 */
        else
            digits[i] = low[low_len - i];      /* reverse order of low digits */
    }
    /* Mirror the left half to form the right half */
    for (size_t i = m; i < L; ++i)
        digits[i] = digits[L - 1 - i];

    /* Convert the base‑9 number to decimal (seq_t) */
    seq_t s = 0;
    for (size_t i = 0; i < L; ++i)
        s = s * 9 + digits[i];

    /* Return the required value */
    return (s + 2) / 9;
}
