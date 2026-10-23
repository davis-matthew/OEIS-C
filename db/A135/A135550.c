/*
 * A135550  Number of bases b, 1 < b < n-1, in which n is a palindrome,
 *          allowing leading zeros when testing if a number is a palindrome.
 * https://oeis.org/A135550
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A135550: Count of bases b (1 < b < n-1) where n is a palindrome
   when leading zeros are allowed.  Equivalently,
   after removing all trailing zeros from the base‑b representation of n,
   the remaining digits form a palindrome.  This uses only integer
   arithmetic. */

#include <stdbool.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    /* For n < 4 no bases satisfy 1 < b < n-1 */
    if (n <= 3)
        return (seq_t)0;

    seq_t count = 0;

    /* Digits of the (trimmed) number are at most 14 in base 2 for n<=10000,
       but in a larger base a digit can be as large as n-2 <= 10000.
       uint16_t holds any possible digit value. */
    uint16_t digs[64];

    for (idx_t b = 2; b + 1 < n; ++b) {
        idx_t temp = n;

        /* Remove trailing zeros: while divisible by base b */
        while (temp % b == 0) {
            temp /= b;
        }

        /* Extract digits of the trimmed number (least significant first) */
        idx_t m = 0;
        while (temp > 0 && m < (idx_t)sizeof(digs) / sizeof(digs[0])) {
            digs[m++] = (uint16_t)(temp % b);
            temp /= b;
        }

        if (temp > 0) /* shouldn't happen for n <= 10000 */
            continue;

        /* Check if remaining digits form a palindrome */
        bool pal = true;
        for (idx_t i = 0; i < m / 2; ++i) {
            if (digs[i] != digs[m - 1 - i]) {
                pal = false;
                break;
            }
        }

        if (pal)
            ++count;
    }

    return count;
}
