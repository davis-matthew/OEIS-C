/*
 * A029958  Numbers that are palindromic in base 13.
 * https://oeis.org/A029958
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 32, u16 n <= 556, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   pow13
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Palindromes in base 13, enumerated in ascending order. */
#include <stdint.h>
#include <stdbool.h>
#include "../utils/pow13.h"

seq_t a(idx_t n)
{
    /* offset 1: first terms are 0,1,…,12 */
    if (n <= 13) {
        return (seq_t)(n - 1);
    }

    /* index among palindromes of length >= 2 (1‑based) */
    idx_t k = n - 13;

    for (int l = 2; ; ++l) {
        int halfLen = (l + 1) / 2;
        idx_t countLen = 12 * pow13(halfLen - 1);
        if (k > countLen) {
            k -= countLen;
            continue;
        }

        /* 0‑based index within this length */
        idx_t index = k - 1;
        idx_t halfStart = pow13(halfLen - 1);      /* leading digit 1 */
        idx_t halfVal = halfStart + index;

        /* extract halfLen digits (most significant first) */
        uint8_t halfDigits[10];
        for (int i = halfLen - 1; i >= 0; --i) {
            halfDigits[i] = halfVal % 13;
            halfVal /= 13;
        }

        /* build full palindrome value */
        seq_t val = 0;
        for (int i = 0; i < halfLen; ++i) {
            val = val * 13 + halfDigits[i];
        }
        if (l % 2 == 0) {
            for (int i = halfLen - 1; i >= 0; --i) {
                val = val * 13 + halfDigits[i];
            }
        } else {
            for (int i = halfLen - 2; i >= 0; --i) {
                val = val * 13 + halfDigits[i];
            }
        }
        return val;
    }
}
