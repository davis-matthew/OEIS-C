/*
 * A118919  Triangle read by rows: T(n,k) is the number of Grand Dyck paths of
 *          semilength n that cross downwards the x-axis k times. (A Grand
 *          Dyck path of semilength n is a path in the half-plane x>=0,
 *          starting at (0,0), ending at (2n,0) and consisting of steps
 *          u=(1,1) and d=(1,-1)).
 * https://oeis.org/A118919
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   a(n) = flatten(Triangle T(n,k) where T(r,k) = (2k+1) *
 *            binomial(2r+2, r-2k) / (r+1). The sequence is obtained by
 *            reading the triangle row‑wise (row r contains 1+floor(r/2)
 *            entries)
 * Range:     u8 n <= 5, u16 n <= 15, u32 n <= 46, u64 n <= 46, u128 n <= 46,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    /* Find the row r that contains the n-th element (0‑based). */
    idx_t total = 0;      /* number of elements before the current row */
    idx_t r = 0;          /* current row index */

    for (;;) {
        idx_t t = 1 + r / 2;          /* length of row r */
        if (total + t > n) break;     /* n lies in this row */
        total += t;
        r++;
    }

    idx_t k = n - total;            /* column index within row r */

    /* Evaluate T(r,k) = (2k+1) * binomial(2r+2, r-2k) / (r+1). */
    seq_t bin  = binomial(2 * r + 2, r - 2 * k);
    seq_t num  = (seq_t)(2 * k + 1) * bin;
    seq_t denom = (seq_t)(r + 1);
    return num / denom;
}
