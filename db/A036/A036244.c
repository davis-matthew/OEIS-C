/*
 * A036244  Denominator of continued fraction given by C(n) = [1; 3, 5, 7,
 *          ..., (2*n-1)].
 * https://oeis.org/A036244
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, frac
 * Formula:   a(n) = a(n-1)*(2*n-1) + a(n-2); a(0) = 0, a(1) = 1
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 10, u64 n <= 17, u128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;

    seq_t prev2 = 0;   /* a(0) */
    seq_t prev1 = 1;   /* a(1) */

    if (n == 1)
        return prev1;

    for (idx_t i = 2; i <= n; ++i) {
        seq_t factor = ((seq_t)i * 2) - 1;      /* 2*i - 1 */
        seq_t curr   = prev1 * factor + prev2;   /* recurrence */
        prev2 = prev1;
        prev1 = curr;
    }

    return prev1;
}
