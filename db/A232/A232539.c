/*
 * A232539  Triangle read by rows: T(n,k) = number of partitions of n into at
 *          most four parts in which the largest part is equal to k, 0 <= k <=
 *          n.
 * https://oeis.org/A232539
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = number of partitions of n into at most four parts;
 *            enumerated by flattening the triangle T(n,k) over rows n and
 *            columns k
 * Range:     u8 n <= 90, u16 n <= 90, u32 n <= 90, u64 n <= 90, u128 n <= 90,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Step 1: locate the row r in the triangular indexing such that
       r*(r+1)/2 <= n < (r+1)*(r+2)/2.  The row length is r+1. */
    idx_t r = 0;
    while ((r + 1) * (r + 2) / 2 <= n) {
        ++r;
    }
    /* Column index within the row */
    idx_t k = n - r * (r + 1) / 2;

    /* Step 2: compute T(r,k) = number of partitions of r into at most
       four parts whose largest part is exactly k.
       By conjugation this equals the number of partitions of r into
       exactly k parts using parts of size 1..4. */
    seq_t cnt = 0;
    idx_t max4 = r / 4;
    for (idx_t x4 = 0; x4 <= max4; ++x4) {
        idx_t rem4 = r - 4 * x4;
        idx_t max3 = rem4 / 3;
        for (idx_t x3 = 0; x3 <= max3; ++x3) {
            idx_t rem3 = rem4 - 3 * x3;
            idx_t max2 = rem3 / 2;
            for (idx_t x2 = 0; x2 <= max2; ++x2) {
                idx_t rem = rem3 - 2 * x2;           /* number of 1's */
                idx_t parts = rem + x2 + x3 + x4;    /* total number of parts */
                if (parts == k) {
                    cnt++;
                }
            }
        }
    }
    return cnt;
}
