/*
 * A176346  A dual remainder symmetrical triangle sequence T(n, m) = 1 +
 *          2*(n+1) - (m+1)*floor((n+1)/(m+1)) - (n-m+1)*floor((n+1)/(
 *          n-m+1)), read by rows.
 * https://oeis.org/A176346
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 5150, u16 n <= 5150, u32 n <= 5150, u64 n <= 5150,
 *            u128 n <= 5150, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Sequence A176346: dual remainder symmetrical triangle sequence.
 * Computation: Flatten the triangle T(n, m) = 1 + 2*(n+1)
 *               - (m+1)*floor((n+1)/(m+1))
 *               - (n-m+1)*floor((n+1)/(n-m+1))
 * by rows (row 0 has 1 entry, row 1 has 2, etc.).
 * a(n) is the n-th entry (offset 0).
 */

#include <stdint.h>

seq_t a(idx_t n) {
    /* Find the row (r) and column (m) for index n. */
    idx_t r = 0;          /* current row being examined */
    idx_t prefix = 0;     /* count of elements before row r */

    /* Accumulate rows until the cumulative count exceeds n. */
    while (prefix + (r + 1) <= n) {
        prefix += r + 1;
        r++;
    }

    idx_t m = n - prefix; /* column within the row r */

    /* Compute the triangle value T(r, m). */
    seq_t n1  = (seq_t)r + 1;            /* n + 1 */
    seq_t m1  = (seq_t)m + 1;            /* m + 1 */
    seq_t nm1 = (seq_t)r - (seq_t)m + 1; /* n - m + 1 */

    seq_t term = (seq_t)1 + (seq_t)2 * n1
                 - m1 * (n1 / m1)
                 - nm1 * (n1 / nm1);

    return term;
}
