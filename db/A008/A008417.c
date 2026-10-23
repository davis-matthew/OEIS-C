/*
 * A008417  Crystal ball sequence for 8-dimensional cubic lattice.
 * https://oeis.org/A008417
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = D(8,n) where D(m,n) satisfies * D(m,0) = D(0,n) = 1 *
 *            D(m,n) = D(m-1,n) + D(m,n-1) + D(m-1,n-1) * for m,n ≥ 1. The
 *            first terms give * a(0) = 1, a(1) = 17, a(2) = 145, …
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 29, u64 n <= 481, u128 n <= 1000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    static seq_t prev[9], curr[9];
    /* initialise D(m,0) = 1 for all m */
    for (idx_t m = 0; m < 9; ++m)
        prev[m] = 1;

    if (n == 0)
        return 1;  /* a(0) = 1 */

    for (idx_t i = 1; i <= n; ++i) {
        curr[0] = 1;                 /* D(0,i) = 1 */
        for (idx_t m = 1; m < 9; ++m) {
            curr[m] = prev[m] + curr[m - 1] + prev[m - 1];
        }
        for (idx_t m = 0; m < 9; ++m)
            prev[m] = curr[m];
    }

    return prev[8];
}
