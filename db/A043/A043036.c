/*
 * A043036  Base 10 palindromes that start with 1.
 * https://oeis.org/A043036
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = the n-th base‑10 palindrome whose first digit is 1,
 *            ordered by increasing value
 * Range:     u8 n <= 12, u16 n <= 122, u32 n <= 2222, u64 n <= 2222,
 *            u128 n <= 2222, big any n
 * Helpers:   reverse_digits, seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/seq_pow.h"
#include "../utils/reverse_digits.h"

seq_t a(idx_t n)
{
    idx_t remaining = n - 1;          /* 0‑based index inside current length */
    idx_t len = 1;                    /* current length of palindromes */

    /* Find the length that contains the n‑th palindrome */
    for (;;) {
        idx_t half = (len + 1) / 2;
        seq_t count_seq = seq_pow(10, half - 1);
        idx_t count = (idx_t)count_seq;   /* count fits in idx_t */

        if (remaining < count)
            break;

        remaining -= count;
        ++len;
    }

    /* Build the palindrome of length len at position 'remaining' within that length */
    idx_t half = (len + 1) / 2;
    seq_t offset   = (seq_t)remaining;
    seq_t power    = seq_pow(10, half - 1);
    seq_t left     = power + offset;          /* left half (most significant digits) */

    seq_t result;
    if ((len & 1) == 0) {                     /* even length */
        seq_t mult = seq_pow(10, half);
        seq_t rev = reverse_digits(left);
        result = left * mult + rev;
    } else {                                  /* odd length */
        seq_t mult = seq_pow(10, half - 1);
        seq_t rev = reverse_digits(left / 10); /* remove the middle digit */
        result = left * mult + rev;
    }

    return result;
}
