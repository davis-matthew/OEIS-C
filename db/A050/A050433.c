/*
 * A050433  Length of longest palindromic subword of (n base 5).
 * https://oeis.org/A050433
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = length of longest palindromic substring of the base‑5 *
 *            representation of n. (n >= 1, a(1) = 1)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    /* Base‑5 digits: most significant digit first.  */
    /* 5^27 > 2^64, so at most 27 digits are needed for uint64_t. */
    unsigned char digits[64];
    int len = 0;
    idx_t tmp = n;

    if (tmp == 0) {
        digits[0] = 0;
        len = 1;
    } else {
        while (tmp > 0) {
            digits[len++] = tmp % 5;  /* least significant first */
            tmp /= 5;
        }
        /* Reverse to most significant first */
        for (int i = 0; i < len / 2; ++i) {
            unsigned char c = digits[i];
            digits[i] = digits[len - 1 - i];
            digits[len - 1 - i] = c;
        }
    }

    if (len == 0)
        return (seq_t)0;          /* should not happen for n >= 1 */

    int maxLen = 1;               /* at least one digit is a palindrome of length 1 */

    for (int i = 0; i < len; ++i) {
        /* Odd length palindromes centered at i */
        int l = i - 1, r = i + 1;
        int curLen = 1;
        while (l >= 0 && r < len && digits[l] == digits[r]) {
            curLen += 2;
            --l; ++r;
        }
        if (curLen > maxLen)
            maxLen = curLen;

        /* Even length palindromes centered between i and i+1 */
        l = i; r = i + 1;
        curLen = 0;
        while (l >= 0 && r < len && digits[l] == digits[r]) {
            curLen += 2;
            --l; ++r;
        }
        if (curLen > maxLen)
            maxLen = curLen;
    }

    return (seq_t)maxLen;
}
