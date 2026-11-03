/*
 * A115111  Number of different ways to select n elements from four sets of n
 *          elements under the precondition of choosing at least one element
 *          from each set.
 * https://oeis.org/A115111
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = binomial(4*n, n)-4*(binomial(3*n, n)+1)+6*binomial(2*n,
 *            n)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 10, u64 n <= 19, u128 n <= 19,
 *            big any n
 * Validated: partial, pass, 19 terms through n = 19 (2026-11-01)
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    if (n < 1)
        return 0;   /* offset starts at 1 */

    seq_t t1 = binomial(4 * n, n);
    seq_t t2 = binomial(3 * n, n);
    t2 += 1;           /* binomial(3*n, n) + 1 */
    t2 *= 4;           /* 4 * (binomial(3*n, n) + 1) */
    seq_t t3 = binomial(2 * n, n);
    t3 *= 6;           /* 6 * binomial(2*n, n) */

    return t1 - t2 + t3;
}
