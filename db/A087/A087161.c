/*
 * A087161  Records in A087159, i.e., A087159(a(n)) = n, and satisfies the
 *          recurrence a(n+3) = 5*a(n+2) - 6* a(n+1) + 2*a(n) with a(1) = 1,
 *          a(2) = 2, and a(3) = 4.
 * https://oeis.org/A087161
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 5*a(n-1) - 6*a(n-2) + 2*a(n-3) for n ≥ 4, with a(1) = 1,
 *            a(2) = 2, a(3) = 4
 * Range:     u8 n <= 6, u16 n <= 11, u32 n <= 20, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Return the explicitly known first terms */
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)2;
    if (n == 3) return (seq_t)4;

    /* prev3 = a(n-3), prev2 = a(n-2), prev1 = a(n-1) */
    seq_t prev3 = (seq_t)1; /* a(1) */
    seq_t prev2 = (seq_t)2; /* a(2) */
    seq_t prev1 = (seq_t)4; /* a(3) */
    seq_t cur = (seq_t)0;

    for (idx_t i = 4; i <= n; ++i)
    {
        cur = prev1 * (seq_t)5 - prev2 * (seq_t)6 + prev3 * (seq_t)2;
        prev3 = prev2;
        prev2 = prev1;
        prev1 = cur;
    }
    return cur;
}
