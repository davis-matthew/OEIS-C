/*
 * A181761  Start with 18; write down the sum of its digits; add last two
 *          terms; repeat.
 * https://oeis.org/A181761
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 50, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   digit_sum
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Sequence A181761
 *
 *  a(1) = 18
 *  a(2) = 9
 *  For n ≥ 3:
 *      if n is odd:   a(n) = a(n-2) + a(n-1)
 *      if n is even:  a(n) = digit_sum(a(n-1))
 *
 *  The implementation evaluates the recurrence iteratively; no caching
 *  is required because n ≤ 10 000 ensures an O(n) algorithm is fast
 *  enough for all builds.
 */
#include "../utils/digit_sum.h"

seq_t a(idx_t n)
{
    if (n == 1)
        return (seq_t)18;
    if (n == 2)
        return (seq_t)9;

    seq_t a_prev2 = (seq_t)18;   /* a(1) */
    seq_t a_prev1 = (seq_t)9;    /* a(2) */
    seq_t a_curr  = 0;

    for (idx_t i = 3; i <= n; ++i)
    {
        if (i & 1)          /* odd index */
            a_curr = a_prev2 + a_prev1;
        else                /* even index */
            a_curr = digit_sum(a_prev1);

        a_prev2 = a_prev1;
        a_prev1 = a_curr;
    }
    return a_prev1;
}
