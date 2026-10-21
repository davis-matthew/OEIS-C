/*
 * A051523  Generalized Stirling number triangle of first kind read by rows:
 *          T(n, k) = [x^k] Product_{m=1..n} (x - m - r), with r = 9.
 * https://oeis.org/A051523
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy, tabl
 * Formula:   not stated
 * Range:     i8 n <= 5, i16 n <= 14, i32 n <= 44, i64 n <= 135, i128 n <= 434,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) is the flattening of the triangle T(n,k)
 * defined by T(0,0)=1 and
 *   T(n,k) = T(n-1,k-1) - (n+9)*T(n-1,k)
 * for n ≥ k ≥ 0, and T(n,k)=0 for n<k or k<0.
 * The provided function returns the n‑th element of this flattened
 * triangle (read row by row, left to right).  The recurrence
 * is computed using only +, -, *, /, % and the built‑in integer
 * types; no floating point or external libraries are required.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    /* Special case: first element */
    if (n == 0) return (seq_t)1;

    /* Find the row r such that r*(r+1)/2 <= n < (r+1)*(r+2)/2 */
    idx_t r = 0;
    idx_t cumulative = 0;
    while (1) {
        idx_t next = cumulative + (r + 1);
        if (n < next) break;
        cumulative = next;
        ++r;
    }

    /* Column index within that row */
    idx_t c = n - cumulative;

    /* Maximum possible row we might need (safe bound for array sizes) */
    const idx_t MAX_ROW = 200;  /* > sqrt(2*MAX_IDX) for all realistic n */
    seq_t prev[MAX_ROW];
    seq_t curr[MAX_ROW];
    idx_t prev_len = 1;
    prev[0] = (seq_t)1;          /* T(0,0) = 1 */

    for (idx_t row = 1; row <= r; ++row) {
        idx_t curr_len = row + 1;
        for (idx_t k = 0; k < curr_len; ++k) {
            seq_t left  = (k > 0)   ? prev[k - 1] : (seq_t)0;
            seq_t right = (k < prev_len) ? prev[k] : (seq_t)0;
            curr[k] = left - (seq_t)(row + 9) * right;
        }
        /* copy current row to previous for next iteration */
        for (idx_t k = 0; k < curr_len; ++k) prev[k] = curr[k];
        prev_len = curr_len;
    }

    return prev[c];
}
