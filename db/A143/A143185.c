/*
 * A143185  Triangle read by rows: T(n, k) = (1 + abs(n-2*k))*binomial(n,k),
 *          with T(n, 0) = T(n, n) = 1.
 * https://oeis.org/A143185
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   T(n,k) = (1+abs(n-2*k))*binomial(n,k); a(n) is the nth element
 *            when the triangle is read row by row (rows numbered from 0)
 * Range:     u8 n <= 39, u16 n <= 158, u32 n <= 573, u64 n <= 1325,
 *            u128 n <= 1325, big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    /* Find the row r containing the nth element.
       row_start is the linear index where row r begins. */
    idx_t r = 0;
    idx_t row_start = 0;
    while (row_start + r + 1 <= n) {
        row_start += r + 1;
        r++;
    }
    idx_t k = n - row_start;          /* column within the row */
    if (k == 0 || k == r)             /* edge entries */
        return (seq_t)1;

    idx_t diff = (r > 2 * k) ? (r - 2 * k) : (2 * k - r);
    seq_t mult = (seq_t)(1 + diff);   /* (1+|n-2k|) as seq_t */
    seq_t bin  = binomial(r, k);      /* binomial coefficient */
    return mult * bin;                /* exact multiplication in seq_t */
}
