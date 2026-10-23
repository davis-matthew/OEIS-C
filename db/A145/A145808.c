/*
 * A145808  Non-palindromic balanced numbers: the first and the last half of
 *          digits have the same sum.
 * https://oeis.org/A145808
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy, nice
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_palindrome
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A145808: Non‑palindromic balanced numbers
   The routine scans natural numbers and returns the n‑th term.
   A balanced number has its first and last halves of digits summing
   to the same value.  For odd‑length numbers the middle digit is
   ignored.  Palindromes are excluded. */

#include <stdbool.h>
#include "../utils/is_palindrome.h"   /* is_palindrome(seq_t, int) */

/* Test whether x is a non‑palindromic balanced number. */
static bool is_balanced_nonpal(seq_t x)
{
    /* Exclude palindromes. */
    if (is_palindrome(x, 10))
        return false;

    /* Extract decimal digits of x (least‑significant first). */
    int digits[50];              /* seq_t <= 128 bits -> < 39 decimal digits */
    int pos = 0;
    seq_t t = x;
    while (t != 0) {
        digits[pos++] = (int)(t % 10);
        t /= 10;
    }
    if (pos == 0)                /* x == 0, never called but safe */
        return false;

    int half = pos / 2;          /* middle digit ignored when pos is odd */

    /* Sum first half (most significant) and last half (least significant). */
    int sum1 = 0, sum2 = 0;
    for (int i = 0; i < half; ++i) {
        sum1 += digits[pos - 1 - i]; /* most significant digit */
        sum2 += digits[i];           /* least significant digit */
    }
    return sum1 == sum2;
}

seq_t a(idx_t n)
{
    idx_t count = 0;
    seq_t x = 0;

    /* Scan integers until the n‑th term is found. */
    while (count < n) {
        ++x;
        if (is_balanced_nonpal(x))
            ++count;
    }
    return x;
}
