/*
 * A145580  Eigentriangle, row sums = A000930.
 * https://oeis.org/A145580
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  tabl, sign
 * Formula:   not stated
 * Range:     i8 n <= 55, i16 n <= 55, i32 n <= 55, i64 n <= 55, i128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Solution for OEIS sequence A145580
   Formula (implicit): a(n) is the (i,j)-entry of the triangle
   defined by a(i,j) = M(i,j) * A000930(j), where
   i is the row index and j is the column index in 1‑based
   ordering, the entries of M belong to the repeating pattern
   (1,1,0,-1,0,1,0,-1,0,1,...) in each column.  The flatten
   sequence a(1),a(2),… is obtained by reading this triangle
   row‑wise starting from the first row.
*/

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined, but safe */

    /* Find row r such that r*(r-1)/2 < n <= r*(r+1)/2   */
    idx_t low = 1, high = n, r = n;
    while (low < high) {
        idx_t mid = low + (high - low) / 2;
        __uint128_t t = (__uint128_t)mid * (mid + 1) / 2;
        if (t >= n)
            high = mid;
        else
            low = mid + 1;
    }
    r = low;

    /* Number of elements before row r */
    __uint128_t prev128 = (__uint128_t)(r - 1) * r / 2;
    idx_t prev = (idx_t)prev128;

    /* Column index in this row (1‑based) */
    idx_t k = n - prev;

    /* Compute A000930(k) with its recurrence a(n)=a(n-1)+a(n-3) */
    seq_t a_k;
    if (k == 1 || k == 2)
        a_k = 1;
    else if (k == 3)
        a_k = 2;
    else {
        seq_t a1 = 1, a2 = 1, a3 = 2;
        for (idx_t i = 4; i <= k; ++i) {
            seq_t next = a3 + a1;   /* a(i-1) + a(i-3) */
            a1 = a2;
            a2 = a3;
            a3 = next;
        }
        a_k = a3;
    }

    /* Compute coefficient M(r,k) based on pattern (1,1,0,-1,0,1,0,-1,0,1,...) */
    seq_t coeff;
    idx_t d = r - k;              /* i - j */
    if (d == 0 || d == 1)
        coeff = 1;
    else {
        static const seq_t pattern4[4] = {0, -1, 0, 1};
        idx_t idx = (d - 2) & 3; /* (d-2) % 4, using bitwise AND since 4 is power of 2 */
        coeff = pattern4[idx];
    }

    return coeff * a_k;
}
