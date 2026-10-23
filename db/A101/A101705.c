/*
 * A101705  Numbers n such that n = 12*reversal(n).
 * https://oeis.org/A101705
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 5, u32 n <= 23, u64 n <= 31, u128 n <= 31,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A101705: numbers n such that n = 12 * reverse(n).
   The sequence can be written as a(n) = 540 * p_{n-1},
   where p_k is the k‑th binary palindrome consisting only of 0's and 1's
   with the first and last digit equal to 1.  a(1) = 0. */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* The first term is defined to be 0. */
    if (n == 1)
        return (seq_t)0;

    /* zero‑based index into the binary–palindrome list */
    idx_t idx = n - 2;

    /* Find the length L of the palindrome that contains index 'idx'. */
    idx_t cumulative = 0;
    int L = 0;

    while (1) {
        ++L;
        int half = (L + 1) / 2;                /* number of free positions */
        uint64_t count = 1ULL << (half - 1);    /* palindromes of this length */
        if (cumulative + count > idx)
            break;
        cumulative += count;
    }

    uint64_t offset = idx - cumulative;          /* offset within this length */

    /* Build the decimal digits of the palindrome. */
    char digits[64];
    digits[0] = '1';
    digits[L - 1] = '1';
    int half = (L + 1) / 2;

    for (int i = 1; i < half; ++i) {
        int bit = (offset >> (half - 1 - i)) & 1;
        digits[i] = '0' + bit;
        digits[L - 1 - i] = digits[i];
    }

    /* Convert digit array to the numeric value. */
    seq_t value = 0;
    for (int i = 0; i < L; ++i) {
        value = value * 10 + (seq_t)(digits[i] - '0');
    }

    value *= (seq_t)540;  /* multiply by 540 to obtain the sequence term */
    return value;
}
