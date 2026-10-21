/*
 * A084983  Palindromes made of only prime digits.
 * https://oeis.org/A084983
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = the n-th smallest palindrome that uses only the prime
 *            digits 2,3,5,7
 * Range:     u8 n <= 11, u16 n <= 88, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

static const uint8_t DIGITS[4] = {2,3,5,7};

seq_t a(idx_t n)
{
    /* Find the length of the n-th palindrome */
    idx_t remaining = n;
    idx_t length = 1;
    while (1) {
        idx_t k = (length + 1) / 2;        /* number of independent digits */
        idx_t count = 1;
        for (idx_t i = 0; i < k; ++i)
            count *= 4ULL;                /* 4^k palindromes of this length */
        if (remaining <= count)
            break;
        remaining -= count;
        ++length;
    }

    /* Build the half (first ceil(L/2) digits) of the palindrome */
    idx_t k = (length + 1) / 2;
    uint8_t half[k];
    idx_t idx = remaining - 1;           /* zero‑based index within this length class */
    for (idx_t i = 0; i < k; ++i) {
        half[k - 1 - i] = DIGITS[idx % 4];
        idx /= 4;
    }

    /* Assemble the full palindrome and return it */
    seq_t val = 0;
    for (idx_t i = 0; i < k; ++i)
        val = val * 10 + (seq_t)half[i];

    if ((length & 1) == 0) {             /* even length: mirror all k digits */
        for (idx_t i = k; i > 0; --i)
            val = val * 10 + (seq_t)half[i - 1];
    } else {                              /* odd length: skip the middle digit */
        for (idx_t i = k; i >= 2; --i)
            val = val * 10 + (seq_t)half[i - 2];
    }

    return val;
}
