/*
 * A142240  A triangular sequence from the pattern in row sums of Pascal's
 *          triangle A007318, Eulerian numbers A008292 and A060187:
 *          Delta_diagonal=m; m={0,1,2,3,...k}.
 * https://oeis.org/A142240
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, uned
 * Formula:   Construct a triangle T where * T[1][1] = 1, * for n ≥ 2: T[n][1]
 *            = T[n][n] = 2, * otherwise (2 ≤ m ≤ n-1): * T[n][m] = (m+1) +
 *            (n-m-1)*(m-1). * The index n is taken row‑wise across this
 *            triangle * (row lengths 1,2,3,…). The value a(n) is the nth *
 *            entry in that row‑wise sequence
 * Range:     u8 n <= 55, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compatibility:  seq_t and idx_t are defined by the build, see
 *  the user manual.  This function solely uses integer arithmetic
 *  and works for any unsigned integer size that can hold the
 *  intermediate values. */

#include <stdint.h>          /* for standard integer types */

seq_t a(idx_t n)
{
    /* row 1 special case */
    if (n == (idx_t)1)
        return (seq_t)1;

    /* Find the row r such that r*(r+1)/2 ≥ n.  Use a simple
     * linear search; for n up to 10 000 this costs only
     * O(√n) which is negligible. */
    idx_t r    = (idx_t)1;       /* current row number  */
    idx_t sum  = (idx_t)1;       /* cumulative number of elements up to row r */

    while (sum < n) {
        r++;
        sum += r;
    }

    /* Sum up to previous row */
    idx_t prev = sum - r;        /* sum of first (r-1) rows */
    idx_t col  = n - prev;       /* column index within this row (1‑based) */

    /* Edge values */
    if (col == (idx_t)1 || col == r)
        return (seq_t)2;

    /* Interior cell value */
    seq_t m = (seq_t)col;
    seq_t row = (seq_t)r;
    seq_t val = (m + (seq_t)1) + (row - m - (seq_t)1) * (m - (seq_t)1);
    return val;
}
