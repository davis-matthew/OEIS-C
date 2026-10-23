/*
 * A030148  Odd palindromes in which parity of digits alternates.
 * https://oeis.org/A030148
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 10, u16 n <= 105, u32 n <= 3905, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A030148:
   a(n) = the nth odd‑palindrome whose digits alternate in parity
   (odd, even, odd, even, …), sorted by increasing value. */
#include <stdint.h>

seq_t a(idx_t n)
{
    /* Find the palindrome length (always odd) that contains the nth term. */
    idx_t len = 1;          /* current palindrome length   */
    idx_t count = 5;        /* number of palindromes of that length */
    idx_t remaining = n;    /* how many terms left to skip */

    while (remaining > count) {
        remaining -= count;
        len += 2;          /* next odd length */
        count *= 5;        /* 5 choices per added digit pair */
    }

    /* Zero‑based index of our palindrome within this length. */
    idx_t offset = remaining - 1;
    idx_t half = (len + 1) / 2;          /* number of digits in the left half (including middle) */

    /* Convert offset to base‑5 representation (digits 0‑4). */
    idx_t digits[32] = {0};              /* enough for any n <= 10 000 */
    idx_t tmp = offset;
    for (idx_t i = half; i > 0; --i) {
        digits[i - 1] = tmp % 5;
        tmp /= 5;
    }

    seq_t result = 0;

    /* Build the left half of the palindrome */
    for (idx_t i = 0; i < half; ++i) {
        seq_t digit = (i % 2 == 0)
                      ? (seq_t)(1 + 2 * digits[i])   /* odd positions: 1,3,5,7,9 */
                      : (seq_t)(2 * digits[i]);      /* even positions: 0,2,4,6,8 */
        result = result * 10 + digit;
    }

    /* Mirror the left half (excluding the middle digit) to form the full palindrome */
    for (int i = (int)half - 2; i >= 0; --i) {
        seq_t digit = (i % 2 == 0)
                      ? (seq_t)(1 + 2 * digits[i])
                      : (seq_t)(2 * digits[i]);
        result = result * 10 + digit;
    }

    return result;
}
