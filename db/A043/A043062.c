/*
 * A043062  a(n)=(s(n)+3)/7, where s(n)=n-th base 7 palindrome that starts
 *          with 4.
 * https://oeis.org/A043062
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 9, u16 n <= 42, u32 n <= 42, u64 n <= 42, u128 n <= 42,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A043062:  a(n) = (s(n)+3)/7 where s(n) is the n‑th base‑7 palindrome
   that starts with 4. The function computes the nth term for any n ≥ 1. */
#include <stddef.h>

seq_t a(idx_t n)
{
    /* Find the length of the desired palindrome. */
    idx_t rem = n;            /* remaining index within the current length set */
    size_t len = 1;           /* current palindrome length being examined */

    for (;; ++len) {
        size_t half = (len + 1) / 2;          /* ceil(len/2) */
        seq_t count = 1;                      /* number of palindromes of this length */

        if (len > 1) {
            /* For a fixed length there are 7^(half-1) possibilities
               (the first digit is fixed to 4, remaining half-1 digits free). */
            for (size_t i = 0; i < half - 1; ++i)
                count *= 7;
        }

        if ((seq_t)rem <= count)
            break;                              /* this is the length we need */

        rem -= (idx_t)count;                    /* skip all palindromes of this length */
    }

    /* Build the left half digits of the palindrome. */
    size_t half_len = (len + 1) / 2;            /* ceil(len/2) */
    uint8_t left[32];                          /* enough for lengths up to 64   */
    left[0] = 4;                                /* leading digit fixed */

    if (half_len > 1) {
        idx_t idx = rem - 1;                    /* zero‑based index within this length */
        for (int i = (int)half_len - 1; i > 0; --i) {
            left[i] = idx % 7;
            idx /= 7;
        }
    }

    /* Convert the full palindrome from base 7 to decimal. */
    seq_t value = 0;

    /* First (left half) part */
    for (size_t i = 0; i < half_len; ++i)
        value = value * 7 + (seq_t)left[i];

    /* Second part : mirror of the left half */
    if (len % 2 == 0) {              /* even length */
        for (int i = (int)half_len - 1; i >= 0; --i)
            value = value * 7 + (seq_t)left[i];
    } else {                         /* odd length – skip the central digit once */
        for (int i = (int)half_len - 2; i >= 0; --i)
            value = value * 7 + (seq_t)left[i];
    }

    return (value + 3) / 7;
}
