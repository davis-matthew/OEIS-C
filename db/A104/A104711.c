/*
 * A104711  Triangle T(n,m) = sum_{k=m..n} A001263(k,m).
 * https://oeis.org/A104711
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = sum_{k=m..r} binom(k,m-1)*binom(k,m)/k * where r is the
 *            row containing the n-th triangle entry * (r satisfies (r-1)*r/2
 *            < n <= r*(r+1)/2) and * m = n - (r-1)*r/2 (1 <= m <= r). This is
 *            the entry * T(r,m) of the Triangle T(n,m) defined in A104711
 * Range:     u8 n <= 17, u16 n <= 59, u32 n <= 62, u64 n <= 62, u128 n <= 62,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binom.h"

seq_t a(idx_t n)
{
    /* Find row r and column m such that the n-th entry lies at
     * (row = r, col = m).  The number of entries in rows 1..r-1
     * is (r-1)*r/2 (triangular number).  Since n can be large
     * we use a loop to locate r; the size is only O(sqrt(n)).     */
    idx_t r = 1;
    while ((r + 1) * r / 2 < n)
        ++r;
    idx_t prev_total = (r - 1) * r / 2;   /* entries before row r   */
    idx_t m = n - prev_total;             /* column index (1‑based) */

    /* Sum Narayana numbers N(k,m) for k = m .. r.
     *   N(k,m) = (1/k) * binom(k,m-1) * binom(k,m)
     */
    seq_t sum = 0;
    for (idx_t k = m; k <= r; ++k) {
        seq_t c1 = binom(k, m - 1);   /* C(k, m-1) */
        seq_t c2 = binom(k, m);       /* C(k, m)   */
        seq_t prod = c1 * c2;        /* product may be large */
        seq_t term = prod / (seq_t)k;/* exact division */
        sum += term;
    }
    return sum;
}
