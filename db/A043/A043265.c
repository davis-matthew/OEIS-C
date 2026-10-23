/*
 * A043265  Sum of the digits of the n-th base 6 palindrome.
 * https://oeis.org/A043265
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = sum of the base‑6 digits of the n‑th base‑6 palindrome
 * Range:     u8 n <= 74, u16 n <= 74, u32 n <= 74, u64 n <= 74, u128 n <= 74,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the n‑th element of OEIS sequence A043265.
   n is 1‑based.  Supports any integer type seq_t, idx_t as defined
   by the build system. */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Handle the single‑digit palindromes 0 … 5 */
    if (n <= 6)
        return (seq_t)(n - 1);

    idx_t rem = n - 6;            /* remaining position after 1‑digit palindromes */
    idx_t len = 2;                /* current palindrome length */

    /* Find the length where the n‑th palindrome lies */
    while (1) {
        idx_t k = (len + 1) / 2;          /* number of digits in the first half */
        idx_t pow6 = 1;
        for (idx_t i = 1; i < k; ++i)    /* compute 6^(k-1) */
            pow6 *= 6ULL;

        idx_t count = 5 * pow6;          /* number of palindromes of this length */

        if (rem <= count)                /* found the correct length */
            break;

        rem -= count;                    /* skip this block */
        ++len;                           /* try next length */
    }

    /* Position within the block of palindromes of length 'len' */
    idx_t idx0 = rem - 1;               /* 0‑based index within the block */
    idx_t k = (len + 1) / 2;            /* half‑length (ceil) */

    /* First digit (most significant) cannot be zero */
    idx_t pow6 = 1;
    for (idx_t i = 1; i < k; ++i)       /* 6^(k-1) */
        pow6 *= 6ULL;

    seq_t first_digit = 1 + idx0 / pow6;           /* in 1 … 5            */
    seq_t rest          = idx0 % pow6;            /* encode remaining k-1 digits  */

    /* Least significant digit of the first half (middle digit for odd length) */
    seq_t middle_digit = rest % 6;
    rest /= 6;

    /* Remaining digits of the first half (most significant to 2nd least) */
    seq_t sum_rest = 0;
    for (idx_t pos = 0; pos < (k > 1 ? k - 2 : 0); ++pos) {
        seq_t digit = rest % 6;
        rest /= 6;
        sum_rest += digit;
    }

    /* Sum of all digits in the first half */
    seq_t sum_first = first_digit + sum_rest + middle_digit;

    /* Final digit sum of the full palindrome */
    seq_t result;
    if (len % 2 == 0)                /* even length */
        result = sum_first * 2;
    else                             /* odd length */
        result = sum_first * 2 - middle_digit;

    return result;
}
