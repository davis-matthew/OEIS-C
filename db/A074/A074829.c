/*
 * A074829  Triangle formed by Pascal's rule, except that the n-th row begins
 *          and ends with the n-th Fibonacci number.
 * https://oeis.org/A074829
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 58, u16 n <= 178, u32 n <= 610, u64 n <= 2306,
 *            u128 n <= 7260, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A074829: 
   The triangle is built as follows:
   row 1:                 1
   row 2:                 1  1
   row r (r ≥ 3):  the first and last entries are the r‑th Fibonacci number;
                   each interior entry is the sum of the two adjacent entries of
                   the previous row.
   The sequence a(n) is the terms of this triangle written row by row.
*/

#include <stdbool.h>

/* maximum number of rows that will ever be needed for the expected input
   range (n up to about 10000 → r ≈ 141, so 2000 is comfortably large) */
#define MAX_ROWS 2000

/* store the triangle and Fibonacci numbers */
static seq_t triangle[MAX_ROWS][MAX_ROWS];
static seq_t fib[MAX_ROWS];

/* how many rows/Fibonacci numbers have been computed */
static idx_t rows_generated = 0;
static idx_t fibs_generated = 0;

/* Ensure that rows up to target_r are computed */
static void ensure_rows(idx_t target_r) {
    if (target_r <= rows_generated) return;

    /* initialize the first two Fibonacci numbers if not yet done */
    if (fibs_generated == 0) {
        fib[1] = (seq_t)1;
        fib[2] = (seq_t)1;
        fibs_generated = 2;
    }

    for (idx_t r = rows_generated + 1; r <= target_r; ++r) {
        /* extend Fibonacci sequence if needed */
        if (fibs_generated < r) {
            for (idx_t i = fibs_generated + 1; i <= r; ++i) {
                fib[i] = fib[i - 1] + fib[i - 2];
            }
            fibs_generated = r;
        }

        /* build row r */
        seq_t *row = triangle[r - 1];       /* zero‑based index for rows */
        row[0] = fib[r];                   /* first entry */
        if (r > 1) {
            seq_t *prev = triangle[r - 2];
            for (idx_t i = 1; i < r - 1; ++i) {
                row[i] = prev[i - 1] + prev[i];  /* interior sums */
            }
        }
        row[r - 1] = fib[r];               /* last entry */

        rows_generated = r;
    }
}

/* return the n‑th term of the sequence (n >= 1) */
seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;  /* undefined but safe return */

    /* find the row containing the n‑th element: r such that r(r+1)/2 >= n */
    idx_t r = 1;
    while ((r * (r + 1)) / 2 < n) ++r;

    ensure_rows(r);

    /* position within the row (0‑based) */
    idx_t prev_total = (r - 1) * r / 2;
    idx_t pos = n - prev_total - 1;

    return triangle[r - 1][pos];
}
