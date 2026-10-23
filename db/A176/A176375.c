/*
 * A176375  x-values in the solution to x^2-69*y^2=1.
 * https://oeis.org/A176375
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 15550*a(n-1) - a(n-2), with a(1)=1, a(2)=7775
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 3, u64 n <= 5, u128 n <= 10,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0; /* out-of-range index, not used */
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)7775;

    seq_t prev2 = (seq_t)1;    /* a(1) */
    seq_t prev1 = (seq_t)7775; /* a(2) */
    seq_t cur;

    for (idx_t i = 3; i <= n; ++i) {
        cur = (seq_t)15550 * prev1 - prev2;
        prev2 = prev1;
        prev1 = cur;
    }
    return prev1;
}
