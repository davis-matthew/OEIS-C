/*
 * A062913  Non-palindromic number and its reversal are both multiples of 14.
 * https://oeis.org/A062913
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 36, u32 n <= 40, u64 n <= 40, u128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Sequence A062913: return the n-th non‑palindromic number such that
 * both the number and its decimal reversal are multiples of 14.
 * The sequence starts with a(1) = 2072.
 * The algorithm tests successive multiples of 14, computes the
 * decimal reversal, and counts those that also satisfy the divisor
 * condition and are not palindromic.
 */

#include <stdbool.h>

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;          /* undefined for n < 1, return 0 */

    idx_t found = 0;
    for (seq_t x = 14; ; x += 14) {
        /* compute decimal reversal of x */
        seq_t rev = 0;
        seq_t t = x;
        while (t > 0) {
            rev = rev * 10 + (t % 10);
            t /= 10;
        }

        /* check divisor condition and non‑palindromic */
        if (rev % 14 == 0 && rev != x) {
            ++found;
            if (found == n)
                return x;
        }
    }
}
