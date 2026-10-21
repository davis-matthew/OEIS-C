/*
 * A065076  a(0) = 0, a(1) = 1, a(n) = (sum of digits of a(n-1)) + a(n-2).
 * https://oeis.org/A065076
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(0) = 0, a(1) = 1, a(n) = digit_sum(a(n-1)) + a(n-2) (for n ≥
 *            2) where digit_sum(x) = sum of decimal digits of x. This
 *            function recomputes the sequence on every call, but stays O(n)
 *            and works for arbitrary‑precision seq_t as well as fixed‑size
 *            types
 * Range:     u8 n <= 57, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   digit_sum
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/digit_sum.h"

seq_t a(idx_t n)
{
    /* Handle the base cases directly. */
    if (n == 0) return (seq_t)0;
    if (n == 1) return (seq_t)1;

    /* Use two variables to keep just the last two computed values. */
    seq_t prev2 = (seq_t)0; /* a(n-2) */
    seq_t prev1 = (seq_t)1; /* a(n-1) */
    seq_t cur;              /* a(n) */

    /* Iterate from 2 up to the requested index. */
    for (idx_t i = 2; i <= n; ++i)
    {
        seq_t sum_digits = digit_sum(prev1);
        cur = sum_digits + prev2;  /* a(i) = digit_sum(a(i-1)) + a(i-2) */

        /* Shift the window for the next iteration. */
        prev2 = prev1;
        prev1 = cur;
    }

    return prev1;
}
