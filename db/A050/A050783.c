/*
 * A050783  Palindromes containing no pair of consecutive equal digits.
 * https://oeis.org/A050783
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = nth palindrome in base 10 with no two consecutive equal
 *            digits. Generation is performed by constructing the odd‑length
 *            palindromes digit by digit. Only odd lengths are possible;
 *            length 1 has 10 terms, and for length (2k−1) there are 9^k
 *            palindromes. The nth term is found by determining its...
 * Range:     u8 n <= 24, u16 n <= 546, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   digit_from_index
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/digit_from_index.h"

seq_t a(idx_t n) {
    static seq_t pow9[64];      /* pow9[i] = 9^i, i up to 63 */
    static int init = 0;
    if (!init) {
        pow9[0] = 1;
        for (int i = 1; i < 64; ++i) pow9[i] = pow9[i - 1] * 9;
        init = 1;
    }

    /* Handle the first 10 terms (length 1 palindromes, 0‑9). */
    if (n <= 10) {
        return (seq_t)(n - 1);   /* 0…9 */
    }

    /* Find the odd length 2k-1 that contains the nth term. */
    idx_t k = 1;                 /* current k (length = 2k-1) */
    seq_t cumulative = 10;       /* total palindromes up to k=1 */

    while (1) {
        ++k;                     /* consider next odd length (k>=2) */
        seq_t count = pow9[k];    /* number of palindromes of this length */
        if (cumulative + count >= n) break;
        cumulative += count;
    }

    /* Offset within this length block (zero‑based). */
    seq_t offset = (seq_t)n - cumulative - 1;

    /* First digit: 1‑9, each leading to 9^(k-1) palindromes. */
    seq_t pow_k_minus1 = pow9[k - 1];
    seq_t first_idx = offset / pow_k_minus1;     /* index of first digit */
    int   first_digit = 1 + (int)first_idx;      /* 1‑9 */
    seq_t remaining = offset % pow_k_minus1;

    uint8_t a_digits[64];         /* store the first k digits */
    a_digits[0] = (uint8_t)first_digit;
    int prev = first_digit;

    /* Determine the remaining k-1 digits from remaining offset. */
    for (int pos = 1; pos < (int)k; ++pos) {
        int exp = (int)k - 1 - pos;          /* remaining positions after this */
        seq_t step = pow9[exp];
        seq_t idx = remaining / step;       /* 0‑8 */
        remaining %= step;

        int digit = digit_from_index(prev, (int)idx);
        a_digits[pos] = (uint8_t)digit;
        prev = digit;
    }

    /* Build the full palindrome value. */
    seq_t val = 0;
    for (int i = 0; i < (int)k; ++i) {
        val = val * 10 + a_digits[i];
    }
    for (int i = (int)k - 2; i >= 0; --i) {
        val = val * 10 + a_digits[i];
    }

    return val;
}
