/*
 * A123125  Triangle of Eulerian numbers T(n,k), 0 <= k <= n, read by rows.
 * https://oeis.org/A123125
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, tabl, changed
 * Formula:   not stated
 * Range:     u8 n <= 23, u16 n <= 48, u32 n <= 110, u64 n <= 261,
 *            u128 n <= 644, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) is obtained from the triangle of Eulerian numbers.
   Row r (r ≥ 1) is [0, E(r,0), E(r,1), …, E(r,r‑1)], where
   E(r,m) is the Eulerian number of order r with m ascents.
   Row 0 consists of a single 1.  The sequence is the rows of
   this triangle concatenated.  For a given index n we locate the
   corresponding row and return either the leading 0 or the
   appropriate Eulerian entry.  All arithmetic is performed in seq_t. */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* locate row r such that (r*(r+1))/2 ≤ n < ((r+1)*(r+2))/2   */
    idx_t r = 0;
    while (((r + 1) * (r + 2)) / 2 <= n) {
        r++;
    }

    /* start index of the row: number of terms in all previous rows */
    idx_t start = (r * (r + 1)) / 2;
    idx_t pos   = n - start;                /* 0‑based position inside the row */

    if (r == 0)                /* row 0 : just 1 */
        return (seq_t)1;

    if (pos == 0)              /* leading 0 for every row after the first */
        return (seq_t)0;

    /* static storage for already computed rows  (index 1..r) */
    static seq_t **rows = NULL;
    static idx_t rows_len = 0;      /* number of rows stored (rows[1] …) */
    static idx_t rows_cap = 0;

    /* enlarge the rows array if necessary */
    if (rows_cap < r + 1) {
        idx_t new_cap = rows_cap ? rows_cap * 2 : 1;
        if (new_cap < r + 1) new_cap = r + 1;
        seq_t **tmp = (seq_t**)realloc(rows, new_cap * sizeof(seq_t*));
        rows = tmp;
        rows_cap = new_cap;
    }

    /* compute any missing rows up to r */
    for (idx_t i = rows_len; i <= r; ++i) {
        if (i == 0) {                /* row 0 has no Eulerian array */
            rows[0] = NULL;
            continue;
        }
        if (i == 1) {                /* base case: E(1,0) = 1 */
            seq_t *arr = (seq_t*)malloc(sizeof(seq_t));
            arr[0] = (seq_t)1;
            rows[1] = arr;
            continue;
        }

        /* i ≥ 2: build Eulerian numbers for row i from row i‑1 */
        seq_t *prev = rows[i - 1];   /* size i‑1 */
        seq_t *curr = (seq_t*)malloc(i * sizeof(seq_t));

        for (idx_t m = 0; m < i; ++m) {
            seq_t term1 = 0;
            if (m < (i - 1))
                term1 = (seq_t)(m + 1) * prev[m];
            seq_t term2 = 0;
            if (m > 0)
                term2 = (seq_t)(i - m) * prev[m - 1];
            curr[m] = term1 + term2;
        }
        rows[i] = curr;
    }
    rows_len = r + 1;

    /* the required value is the (pos-1)-th Eulerian number of row r */
    return rows[r][pos - 1];
}
