/*
 * A054469  a(n) = a(n-1) + a(n-2) + (n+2)*binomial(n+3, 3)/2, with a(0) = 1,
 *          a(1) = 7.
 * https://oeis.org/A054469
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = a(n-1) + a(n-2) + (n+1)*(n+2)^2*(n+3)/12, with a(0) = 1,
 *            a(1) = 7. This function is accurate whenever the result fits in
 *            seq_t; otherwise overflow occurs as per the underlying unsigned
 *            integer type
 * Range:     u8 n <= 4, u16 n <= 13, u32 n <= 35, u64 n <= 81, u128 n <= 174,
 *            big any n
 * Helpers:   term_value
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>   /* for abort() if needed, not used here */
#include "../utils/term_value.h"

seq_t a(idx_t n)
{
    /* Handle the two base cases directly. */
    if (n == 0) return 1;
    if (n == 1) return 7;

    seq_t prev2 = 1;   /* a(0) */
    seq_t prev1 = 7;   /* a(1) */
    seq_t cur = 0;

    for (idx_t i = 2; i <= n; ++i)
    {
        cur = prev1 + prev2 + term_value(i);
        prev2 = prev1;
        prev1 = cur;
    }
    return cur;
}
