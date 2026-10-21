/*
 * A211343  Triangle read by rows: T(n,k), n >= 1, k >= 1, in which column k
 *          lists the positive integers interleaved with k-1 zeros, and the
 *          first element of column k is in row k(k+1)/2.
 * https://oeis.org/A211343
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf, look
 * Formula:   a(n) is obtained by reading the triangle T(r,k) by rows. * For a
 *            given row r (r ≥ 1) let m(r) = greatest integer such that *
 *            m(m+1)/2 ≤ r (the number of columns in that row). * For 1 ≤ k ≤
 *            m(r) define * * T(r,k) = { (r - k(k+1)/2)/k + 1 if (r -
 *            k(k+1)/2) % k == 0 * { 0 otherwise } * * The sequenc...
 * Range:     u8 n <= 3608, u16 n <= 9808, u32 n <= 9808, u64 n <= 9808,
 *            u128 n <= 9808, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>   /* for uint64_t if idx_t is defined as such */

seq_t a(idx_t n)
{
    /* n is 1‑based */
    idx_t cumulative = 0;   /* number of terms processed so far */
    idx_t r = 1;            /* current row */

    while (1) {
        /* determine number of columns in row r: m(r) = max k with k(k+1)/2 ≤ r */
        idx_t m = 0;
        while ((m + 1) * (m + 2) / 2 <= r) {
            ++m;
        }

        /* if the desired index lies in this row, compute it */
        if (cumulative + m >= n) {
            idx_t offset = n - cumulative - 1;   /* 0‑based column index in this row */
            idx_t k = offset + 1;                /* 1‑based column number */

            seq_t s = (seq_t)k * (k + 1) / 2;    /* start row of column k */
            seq_t diff = (seq_t)r - s;

            if (diff % k == 0) {
                return diff / k + 1;
            } else {
                return 0;
            }
        }

        /* otherwise skip this entire row and continue */
        cumulative += m;
        ++r;
    }
}
