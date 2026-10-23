/*
 * A049061  Triangle a(n,k) (1<=k<=n) of "signed Eulerian numbers" read by
 *          rows.
 * https://oeis.org/A049061
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, easy, tabl, nice
 * Formula:   not stated
 * Range:     i8 n <= 40, i16 n <= 110, i32 n <= 262, i64 n <= 683,
 *            i128 n <= 1977, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A049061: Triangle of signed Eulerian numbers read by rows.
   Recurrence:
     D(1,1) = 1
     D(m,k) = D(m‑1,k) – D(m‑1,k‑1)               if m is even
     D(m,k) = k*D(m‑1,k) + (m‑k+1)*D(m‑1,k‑1)   if m is odd
   a(n) is the n-th entry of the concatenation of all rows (1 – ∞). */

#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

/* static storage for the triangle; triangle[r][k] stores D(r,k+1) for 1‑based r,k */
static seq_t **triangle = NULL;
static idx_t  max_row   = 0;         /* highest row that has been computed */

/* Ensure rows 1..row are computed */
static void ensure_rows(idx_t row)
{
    if (row <= max_row) return;

    /* Resize the array of row pointers */
    triangle = (seq_t**)realloc(triangle, (row + 2) * sizeof(seq_t*));
    if (!triangle) return; /* allocation failure; caller will fault */

    for (idx_t r = max_row + 1; r <= row; ++r) {
        seq_t *cur = (seq_t*)malloc(r * sizeof(seq_t));
        triangle[r] = cur;
        if (r == 1) {
            cur[0] = 1;                     /* D(1,1) */
        } else {
            seq_t *prev = triangle[r-1];
            if (r % 2 == 0) {               /* even row: D(m,k)=D(m-1,k)-D(m-1,k-1) */
                for (idx_t k = 1; k <= r; ++k) {
                    seq_t termk   = (k <= r-1) ? prev[k-1] : 0;
                    seq_t termk1  = (k >= 2)   ? prev[k-2] : 0;
                    cur[k-1] = termk - termk1;
                }
            } else {                       /* odd row: D(m,k)=k*D(m-1,k)+(m-k+1)*D(m-1,k-1) */
                for (idx_t k = 1; k <= r; ++k) {
                    seq_t t1 = (k <= r-1) ? prev[k-1] : 0; /* D(m-1,k) */
                    seq_t t2 = (k >= 2)   ? prev[k-2] : 0; /* D(m-1,k-1) */
                    cur[k-1] = (seq_t)k * t1 + (seq_t)(r - k + 1) * t2;
                }
            }
        }
    }
    max_row = row;
}

/* Return the n-th term of the sequence (1‑based).  For n<1, 0 is returned. */
seq_t a(idx_t n)
{
    if (n < 1) return 0;

    /* Find the row r containing the n-th entry: 1 + 2 + … + r >= n  */
    idx_t cum = 0;
    idx_t r   = 0;
    while (cum < n) {
        ++r;
        cum += r;                 /* cum = r(r+1)/2 */
    }
    idx_t prev_cum = cum - r;     /* sum up to row-1 */
    idx_t k = n - prev_cum;      /* 1‑based position within the row */

    ensure_rows(r);
    return triangle[r][k-1];
}
