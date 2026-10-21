/*
 * A099994  Bisection of A002113.
 * https://oeis.org/A099994
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) is the n‑th palindrome whose centre digit is odd *
 *            (single‑digit palindromes are the even numbers 0,2,4,6,8).
 *            Palindromes * are generated in increasing order of length and
 *            value
 * Range:     u8 n <= 18, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

static const unsigned char odd_digits[5] = {1, 3, 5, 7, 9};

seq_t a(idx_t n)
{
    /* first 5 terms : 0,2,4,6,8 */
    if (n <= 5)
        return (seq_t)(2 * (n - 1));

    /* offset of terms already output */
    idx_t offset = 5;

    /* length 2 palindromes: 11,33,55,77,99 */
    if (n <= offset + 5) {
        unsigned char d = odd_digits[n - offset - 1];
        return (seq_t)(d * 11);
    }
    offset += 5;   /* now offset == 10 */

    /* palindromes of length >= 3 */
    for (int len = 3; ; ++len) {
        int k = len / 2;          /* floor(len/2) */
        int prefixLen;
        idx_t pow10 = 1;          /* 10^(prefixLen-1) */
        idx_t count_len;

        if (len & 1) {            /* odd length : 2k+1 */
            prefixLen = k;        /* k digits before centre */
            /* compute 10^(prefixLen-1) */
            for (int i = 1; i < prefixLen; ++i)
                pow10 *= 10;
            count_len = 9 * pow10 * 5;   /* 5 centre options */
        } else {                  /* even length : 2k, k>=2 */
            if (len == 2)          /* already handled above */
                continue;
            prefixLen = k - 1;     /* k-1 digits before centre */
            for (int i = 1; i < prefixLen; ++i)
                pow10 *= 10;
            count_len = 9 * pow10 * 5;
        }

        if (n <= offset + count_len) {
            /* index inside this block (0‑based) */
            idx_t idx = n - offset - 1;
            idx_t outerIdx = idx / 5;
            int centerIndex = idx % 5;
            unsigned char center = odd_digits[centerIndex];

            /* Build the prefix digits */
            unsigned char prefix[3];          /* prefixLen <= 3 for n <= 10000 */
            if (prefixLen > 0) {
                unsigned char first = 1 + outerIdx / pow10;
                idx_t rem = outerIdx % pow10;
                prefix[0] = first;
                for (int pos = prefixLen - 2; pos >= 0; --pos) {
                    prefix[pos + 1] = rem % 10;
                    rem /= 10;
                }
            }

            /* Construct the palindrome */
            seq_t val = 0;
            if (len & 1) {            /* odd length */
                for (int i = 0; i < prefixLen; ++i)
                    val = val * 10 + prefix[i];
                val = val * 10 + center;
                for (int i = prefixLen - 1; i >= 0; --i)
                    val = val * 10 + prefix[i];
            } else {                  /* even length */
                for (int i = 0; i < prefixLen; ++i)
                    val = val * 10 + prefix[i];
                val = val * 10 + center;
                val = val * 10 + center;
                for (int i = prefixLen - 1; i >= 0; --i)
                    val = val * 10 + prefix[i];
            }
            return val;
        }
        offset += count_len;
    }
}
