/*
 * A239313  Triangle read by rows: T(n,k), n>=1, k>=1, in which column k lists
 *          the odd numbers interleaved with k-1 zeros, except the first
 *          column which lists 0 together with the nonnegative integers, and
 *          the first element of column k is in row k*(k+1)/2.
 * https://oeis.org/A239313
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   not stated
 * Range:     u8 n <= 82, u16 n <= 82, u32 n <= 82, u64 n <= 82, u128 n <= 82,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) is the nth element of the triangle of A239313.
   The triangle is read by rows: row r has L(r) columns,
   where L(r) = max{k | k(k+1)/2 ≤ r} = floor((√(8r+1)-1)/2).
   Column 1 contains 0,0,1,2,3,…  (value = max(0, r-2)).
   For k ≥ 2, column k starts at row k(k+1)/2 and repeats
   the pattern: odd, 0,...,0 (k‑1 zeros between odds),
   i.e. at offset t = r - k(k+1)/2, value = (t%k==0) ? 2*(t/k+1)-1 : 0.  */

#include <stdint.h>

/* integer square root, floor of √n, for 0 ≤ n < 2^63 */
static inline idx_t isqrt_u64(idx_t n)
{
    idx_t low = 0, high = n;
    while (low <= high) {
        idx_t mid = (low + high) >> 1;
        idx_t sq  = mid * mid;
        if (sq == n) return mid;
        if (sq < n) {
            if (mid == n) break;
            low = mid + 1;
        } else {
            if (mid == 0) break;
            high = mid - 1;
        }
    }
    return high;
}

/* length of row r (number of columns present in that row) */
static inline idx_t row_length(idx_t r)
{
    idx_t s = isqrt_u64(8 * r + 1);
    return (s - 1) / 2;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;    /* safety, n is 1‑based */

    idx_t cum = 0;   /* total elements of rows processed so far   */
    idx_t r   = 1;   /* current row                               */

    /* find the row that contains the nth element          */
    for (;; ++r) {
        idx_t L = row_length(r);
        if (cum + L >= n) break;
        cum += L;
    }

    /* column position within row r: 1‑based            */
    idx_t offset = n - cum - 1;   /* zero‑based offset in the row */
    idx_t k = offset + 1;          /* column index in this row     */

    if (k == 1) {                   /* special first column        */
        if (r <= 2) return (seq_t)0;
        return (seq_t)(r - 2);
    } else {                         /* k ≥ 2: use interleaved pattern */
        idx_t start_row = k * (k + 1) / 2;   /* first row containing column k */
        idx_t t = r - start_row;            /* offset inside this column */
        if (t % k == 0) {
            idx_t odd_index = t / k + 1;     /* nth odd number (1‑based) */
            return (seq_t)(2 * odd_index - 1);
        } else {
            return (seq_t)0;
        }
    }
}
