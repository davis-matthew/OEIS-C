/*
 * A043087  (s(n)+2)/10, where s(n)=n-th base 10 palindrome that starts with
 *          8.
 * https://oeis.org/A043087
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = (s(n)+2)/10, where s(n) is the n-th base‑10 palindrome
 *            starting with 8
 * Range:     u8 n <= 2, u16 n <= 22, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   power10_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/power10_small.h"

seq_t a(idx_t n)
{
    /* Find the length of the n-th palindrome starting with 8 */
    idx_t m = n;
    int len = 1;
    while (1) {
        seq_t count;
        if (len == 1) {
            count = 1;
        } else if (len % 2 == 0) {
            int k = len / 2;          /* number of digit pairs */
            count = 1;
            for (int i = 0; i < k - 1; ++i) count *= 10; /* independent digits */
        } else {
            int k = (len - 1) / 2;    /* number of independent digits for odd length */
            count = 1;
            for (int i = 0; i < k; ++i) count *= 10;
        }
        if (m <= count) break;
        m -= (idx_t)count;
        ++len;
    }

    /* 0‑based offset within palindromes of this length */
    int offset = (int)(m - 1);

    /* Build the palindrome digit by digit */
    /* max length needed for n up to 10 000 is 9, so 20 is safe */
    uint8_t digits[20] = {0};
    digits[0] = digits[len - 1] = 8;

    if (len > 1) {
        if (len % 2 == 1) { /* odd length */
            int k = (len - 1) / 2; /* number of independent positions including middle */
            for (int i = 0; i < k; ++i) {
                seq_t divisor = power10_small(k - 1 - i);
                int d = (int)((offset / divisor) % 10);
                digits[i + 1] = (uint8_t)d;
                int mir = len - 1 - (i + 1);
                digits[mir] = digits[i + 1];
            }
        } else { /* even length */
            int k = len / 2;
            for (int i = 0; i < k - 1; ++i) {
                seq_t divisor = power10_small((k - 2) - i);
                int d = (int)((offset / divisor) % 10);
                digits[i + 1] = (uint8_t)d;
                int mir = len - 1 - (i + 1);
                digits[mir] = digits[i + 1];
            }
        }
    }

    /* Convert digits to the palindrome value */
    seq_t val = 0;
    for (int i = 0; i < len; ++i) {
        val = val * 10 + digits[i];
    }

    /* a(n) = val/10 + 1 */
    return val / 10 + 1;
}
