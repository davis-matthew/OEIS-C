/*
 * A043273  Sum of the digits of the n-th base 14 palindrome.
 * https://oeis.org/A043273
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = sum of digits of the nth base 14 palindrome
 * Range:     u8 n <= 72, u16 n <= 72, u32 n <= 72, u64 n <= 72, u128 n <= 72,
 *            big any n
 * Helpers:   pow14_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/pow14_u64.h"

seq_t a(idx_t n)
{
    /* Handle the first 14 one‑digit palindromes. */
    if (n <= 14)
        return (seq_t)(n - 1);

    /* Remaining index after the one‑digit palindromes. */
    idx_t m = n - 14;            /* 1‑based within the remaining groups */

    /* Find the length of the palindrome containing this index. */
    idx_t len;
    for (len = 2; ; ++len) {
        idx_t half   = (len + 1) / 2;
        idx_t count  = 13 * pow14_u64(half - 1);
        if (m <= count)
            break;
        m -= count;
    }

    /* Convert the 1‑based index inside this length into a 0‑based rank. */
    idx_t k = m - 1;                         /* 0‑based rank in this group */
    idx_t half      = (len + 1) / 2;          /* number of left‑half digits   */
    idx_t pow14     = pow14_u64(half - 1);    /* 14^(half-1)                 */

    /* Determine the first (most significant) digit of the left half. */
    idx_t first_digit = 1 + (k / pow14);
    idx_t rem         = k % pow14;           /* remaining digits of the left half */

    /* Sum of the digits in the left half. */
    seq_t sum_left   = first_digit;
    seq_t middle     = 0;                    /* digit that will be the middle
                                               of the palindrome in odd length
                                               cases */
    idx_t remaining_digits = half - 1;
    for (idx_t i = 0; i < remaining_digits; ++i) {
        seq_t d = (seq_t)(rem % 14);
        if (len % 2 == 1 && i == 0)          /* first extracted is the
                                               middle digit for odd length */
            middle = d;
        sum_left += d;
        rem /= 14;
    }

    /* Total digit sum of the palindrome. */
    if (len % 2 == 0)
        return 2 * sum_left;                 /* even length: mirror all digits */
    else
        return 2 * sum_left - middle;        /* odd length: middle digit counted once */
}
