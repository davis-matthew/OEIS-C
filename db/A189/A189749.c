/*
 * A189749  a(1)=5, a(2)=5, a(n)=5*a(n-1) + 5*a(n-2).
 * https://oeis.org/A189749
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(1)=5, a(2)=5, a(n)=5*a(n-1)+5*a(n-2) for n≥3
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 13, u64 n <= 25, u128 n <= 51,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>  /* for size_t, used only by the compiler */

seq_t a(idx_t n)
{
    if (n == 1 || n == 2)
        return 5;

    seq_t prev2 = 5;   /* a(n-2)   */
    seq_t prev1 = 5;   /* a(n-1)   */
    seq_t cur;

    for (idx_t i = 3; i <= n; ++i) {
        cur = 5 * prev1 + 5 * prev2;
        prev2 = prev1;
        prev1 = cur;
    }
    return prev1;
}
