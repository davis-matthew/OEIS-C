/*
 * A045571  Numbers that are palindromic, divisible by 11 and have an odd
 *          number of digits.
 * https://oeis.org/A045571
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 58, u32 n <= 9090, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Generates the sequence A045571: the nth palindromic number with an odd
   number of digits that is divisible by 11.  The numbers are generated
   in increasing order by enumerating odd‑length palindromes and
   selecting those that are multiples of 11. */

#include <stdbool.h>

/* integer power of 10 for idx_t type */
static inline idx_t pow10_int(idx_t e)
{
    idx_t r = 1;
    while (e--) r *= 10;
    return r;
}

seq_t a(idx_t n)
{
    /* Count of qualifying numbers found so far */
    idx_t count = 0;

    /* Enumerate odd lengths: 3,5,7,…  (len = 2*k+1, k>=1) */
    for (int k = 1; ; ++k)
    {
        /* Number of digits in first half (including middle digit) */
        idx_t half_start = pow10_int(k);          /* smallest (k+1)-digit number */
        idx_t half_end   = pow10_int(k + 1) - 1;  /* largest  (k+1)-digit number */
        seq_t shift      = (seq_t) pow10_int(k);  /* 10^k */

        for (idx_t half = half_start; half <= half_end; ++half)
        {
            /* Remove the middle digit: half / 10 */
            idx_t temp = half / 10;
            /* Reverse the remaining digits */
            idx_t rev = 0;
            while (temp > 0)
            {
                rev = rev * 10 + temp % 10;
                temp /= 10;
            }

            /* Construct the palindrome */
            seq_t pal = (seq_t)half * shift + (seq_t)rev;

            /* Keep only those divisible by 11 */
            if (pal % 11 == 0)
            {
                ++count;
                if (count == n)
                    return pal;
            }
        }
    }
}
