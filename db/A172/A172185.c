/*
 * A172185  (9,11) Pascal triangle.
 * https://oeis.org/A172185
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = 1 if n = 0 * 9*binom(r,k) + 2*binom(r-1,k-1) otherwise, *
 *            where r = floor((sqrt(1+8n)-1)/2) and k = n - r*(r+1)/2. *
 *            Initial values: a(0)=1, a(1)=9, a(2)=11
 * Range:     u8 n <= 30, u16 n <= 141, u32 n <= 540, u64 n <= 1325,
 *            u128 n <= 1325, big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binom.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return 1;

    /* Find the row r such that r*(r+1)/2 <= n < (r+1)*(r+2)/2 */
    idx_t r = 0;
    while ((r + 1) * (r + 2) / 2 <= n)
        ++r;

    idx_t start = r * (r + 1) / 2;
    idx_t k = n - start;

    seq_t bin1 = binom(r, k);
    seq_t bin2 = (k == 0) ? 0 : binom(r - 1, k - 1);

    return 9 * bin1 + 2 * bin2;
}
