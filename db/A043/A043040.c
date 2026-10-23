/*
 * A043040  Numbers that are palindromic and divisible by 5.
 * https://oeis.org/A043040
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 123, u32 n <= 2223, u64 n <= 2223,
 *            u128 n <= 2223, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A043040: numbers that are palindromic and divisible by 5.
   The first term is 0; all subsequent terms are palindromes that start
   and end with the digit 5, and are listed in increasing numerical order. */

#include <stdbool.h>

seq_t a(idx_t n)
{
    /* Special first term */
    if (n == 1)
        return 0;

    /* Remaining positions start from n-2 (skip the 0 term) */
    idx_t remaining = n - 2;

    /* Find the length of the palindrome that contains the required term */
    idx_t len = 1;
    idx_t cumulative = 0;
    while (1) {
        idx_t count;
        if (len == 1 || len == 2) {
            count = 1;
        } else {
            /* number of independent digits for this length */
            idx_t exp = (len % 2 == 1) ? (len - 1) / 2 : (len / 2) - 1;
            count = 1;
            for (idx_t i = 0; i < exp; ++i)
                count *= 10;
        }
        if (cumulative + count > remaining)
            break;
        cumulative += count;
        ++len;
    }

    idx_t offset = remaining - cumulative;

    /* independent digit count for the found length */
    idx_t indCount;
    if (len == 1 || len == 2)
        indCount = 0;
    else
        indCount = (len % 2 == 1) ? (len - 1) / 2 : (len / 2) - 1;

    /* digit array for the complete palindrome */
    unsigned char d[64] = {0};
    d[0] = d[len - 1] = 5;

    /* extract independent digits of the position number */
    unsigned char ind[32] = {0};
    idx_t tmp = offset;
    for (idx_t i = indCount; i > 0; --i) {
        ind[i - 1] = (unsigned char)(tmp % 10);
        tmp /= 10;
    }

    /* fill the palindrome digits */
    for (idx_t i = 1; i <= indCount; ++i) {
        unsigned char val = ind[i - 1];
        d[i] = val;
        d[len - i - 1] = val;
    }

    /* construct the numeric value of type seq_t */
    seq_t val = 0;
    for (idx_t i = 0; i < len; ++i)
        val = val * 10 + (seq_t)d[i];

    return val;
}
