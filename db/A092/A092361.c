/*
 * A092361  Palindromic numbers containing one or more odd digits.
 * https://oeis.org/A092361
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, base
 * Formula:   a(n) is the n‑th palindromic integer that contains at least one
 *            odd decimal digit
 * Range:     u8 n <= 23, u16 n <= 640, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   has_odd_digit, pow10_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/has_odd_digit.h"
#include "../utils/pow10_seq.h"

/* Build a palindrome from the given prefix. If odd_len is true, the middle
   digit of the prefix is omitted before mirroring. */
static seq_t make_pal(idx_t prefix, bool odd_len)
{
    seq_t pal   = prefix;
    seq_t tmp   = prefix;
    if (odd_len)
        tmp /= 10;                 /* skip the middle digit for odd lengths */
    while (tmp > 0)
    {
        pal = pal * 10 + (tmp % 10);
        tmp /= 10;
    }
    return pal;
}

/* Small power of ten: sequence type. e < about 20 for the required n. */

seq_t a(idx_t n)
{
    idx_t count = 0;

    for (idx_t len = 1; ; ++len)
    {
        /* Determine the number of digits in the left half (prefix). */
        idx_t half = (len % 2 == 0) ? (len / 2) : ((len + 1) / 2);

        /* Prefix ranges from 10^(half-1) to 10^half - 1.  */
        seq_t start_seq = pow10_seq(half - 1);
        seq_t end_seq   = pow10_seq(half) - 1;

        idx_t start = (idx_t)start_seq;
        idx_t end   = (idx_t)end_seq;

        for (idx_t pref = start; pref <= end; ++pref)
        {
            seq_t pal = make_pal(pref, len % 2 == 1);
            if (has_odd_digit(pal))
            {
                ++count;
                if (count == n)
                    return pal;
            }
        }
    }
}
