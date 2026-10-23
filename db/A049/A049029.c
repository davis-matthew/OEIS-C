/*
 * A049029  Triangle read by rows, the Bell transform of the quartic factorial
 *          numbers A007696(n+1) without column 0.
 * https://oeis.org/A049029
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, tabl
 * Formula:   a[row][col] is defined by the recurrence * a(1,1) = 1 * a(n+1,m)
 *            = (4*n + m) * a(n,m) + a(n,m-1) for n >= m >= 1 * a(n,0) = 0,
 *            a(n,m) = 0 if n < m * a(n) is the n‑th element of the triangle
 *            read row‑wise
 * Range:     u8 n <= 6, u16 n <= 15, u32 n <= 36, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

#define MAX_ROWS 200

static seq_t triangle[MAX_ROWS+1][MAX_ROWS+1];
static idx_t max_row = 0;

/* Return the n-th term of OEIS A049029 (1‑based index) */
seq_t a(idx_t n) {
    /* Find the row r such that r*(r+1)/2 ≥ n */
    idx_t r = 1;
    while (r*(r+1)/2 < n) r++;

    /* Column within the row (1‑based) */
    idx_t c = n - (r-1)*r/2;

    /* Initialize first row on first call */
    if (max_row == 0) {
        /* static arrays are zero‑initialized; set only the first non‑zero entry */
        triangle[1][1] = (seq_t)1;
        max_row = 1;
    }

    /* Generate missing rows up to r */
    while (max_row < r) {
        idx_t next = max_row + 1;
        for (idx_t m = 1; m <= next; ++m) {
            seq_t value = (seq_t)0;
            if (m <= max_row) {
                seq_t factor = (seq_t)(4*max_row + m);   /* small integer factor */
                value += factor * triangle[max_row][m];
            }
            if (m > 1) {
                value += triangle[max_row][m-1];
            }
            triangle[next][m] = value;
        }
        max_row = next;
    }

    return triangle[r][c];
}
