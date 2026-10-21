/*
 * A050431  Length of longest palindromic subword of (n base 3).
 * https://oeis.org/A050431
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = length of the longest palindromic contiguous substring in
 *            the base‑3 representation of n
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == (idx_t)0)
    {
        /* Represent 0 as "0".  The longest palindrome has length 1. */
        return (seq_t)1;
    }

    /* Convert n to base 3.  The digits are stored least significant first. */
    uint8_t d[64];
    int len = 0;
    idx_t t = n;
    while (t > (idx_t)0)
    {
        d[len++] = (uint8_t)(t % (idx_t)3);
        t /= (idx_t)3;
    }

    /* Reverse to most significant first for easier palindrome checks. */
    for (int i = 0; i < len / 2; ++i)
    {
        uint8_t tmp = d[i];
        d[i] = d[len - 1 - i];
        d[len - 1 - i] = tmp;
    }

    /* Expand around every center to find the longest palindrome. */
    int best = 1;  /* at least one digit exists */
    for (int center = 0; center < len; ++center)
    {
        /* Odd length palindromes. */
        int l = center, r = center;
        while (l >= 0 && r < len && d[l] == d[r])
        {
            int curr = r - l + 1;
            if (curr > best) best = curr;
            --l; ++r;
        }

        /* Even length palindromes. */
        l = center;
        r = center + 1;
        while (l >= 0 && r < len && d[l] == d[r])
        {
            int curr = r - l + 1;
            if (curr > best) best = curr;
            --l; ++r;
        }
    }

    return (seq_t)best;
}
