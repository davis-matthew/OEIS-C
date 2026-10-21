/*
 * A090171  Triangle read by rows, related to Pascal's triangle read mod 2,
 *          starting with 0, 1, 0.
 * https://oeis.org/A090171
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, tabl
 * Formula:   not stated
 * Range:     u8 n <= 104, u16 n <= 104, u32 n <= 104, u64 n <= 104,
 *            u128 n <= 104, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Triangle A090171 implemented with the recurrence
 *
 *   T(0,0) = 0
 *   T(1,0) = 1 , T(1,1) = 0
 *   For n ≥ 2, k ≥ 0
 *       T(n,k) = T(n-1,k) + T(n-1,k-1) + T(n-2,k)
 *                + T(n-2,k-1) + T(n-2,k-2)   (mod 2)
 *
 *   The sequence a(n) enumerates the triangle row‑by‑row:
 *     a(0) = T(0,0)
 *     a(1) = T(1,0), a(2) = T(1,1)
 *     a(3) = T(2,0), a(4) = T(2,1), a(5) = T(2,2), …
 *
 *   All arithmetic is performed modulo 2, so a(n) is 0 or 1.
 */

#include <stdlib.h>

/* Dynamic array of row pointers; tri_rows[i] points to row i (length i+1) */
static seq_t **tri_rows = NULL;
static size_t tri_rows_used  = 0;   /* number of rows computed so far */
static size_t tri_rows_alloc = 0;   /* allocated capacity */

/* Ensure that rows 0 … r_needed are computed. */
static void ensure_rows(idx_t r_needed)
{
    while (tri_rows_used <= (size_t)r_needed) {
        size_t r = tri_rows_used;
        /* Allocate space for the new row */
        seq_t *row = (seq_t *)malloc((r + 1) * sizeof(seq_t));
        if (!row) abort();             /* out of memory */

        if (r == 0) {                  /* first row */
            row[0] = (seq_t)0;
        } else if (r == 1) {           /* second row */
            row[0] = (seq_t)1;
            row[1] = (seq_t)0;
        } else {                       /* rows from 2 onward */
            seq_t *prev1 = tri_rows[r - 1];
            seq_t *prev2 = tri_rows[r - 2];
            for (size_t k = 0; k <= r; ++k) {
                seq_t val = (seq_t)0;

                /* T(n-1,k) */
                if (k < r)           /* prev1 has indices 0…r-1 */
                    val += prev1[k];

                /* T(n-1,k-1) */
                if (k > 0)           /* prev1[k-1] exists for k>0 */
                    val += prev1[k - 1];

                /* T(n-2,k) */
                if (k < r - 1)       /* prev2 has indices 0…r-2 */
                    val += prev2[k];

                /* T(n-2,k-1) */
                if (k > 0 && k - 1 < r - 1)
                    val += prev2[k - 1];

                /* T(n-2,k-2) */
                if (k > 1 && k - 2 < r - 1)
                    val += prev2[k - 2];

                /* Reduce the value modulo 2 */
                row[k] = val & (seq_t)1;
            }
        }

        /* Grow the array of pointers if necessary */
        if (tri_rows_used == tri_rows_alloc) {
            size_t new_cap = tri_rows_alloc ? tri_rows_alloc * 2 : 8;
            seq_t **tmp = (seq_t **)realloc(tri_rows, new_cap * sizeof(seq_t *));
            if (!tmp) abort();
            tri_rows = tmp;
            tri_rows_alloc = new_cap;
        }

        tri_rows[tri_rows_used++] = row;
    }
}

/* Return the n‑th term of A090171 (0‑indexed). */
seq_t a(idx_t n)
{
    /* Find the row r such that r·(r+1)/2 ≤ n < (r+1)·(r+2)/2. */
    idx_t r = 0;
    while ((r + 1) * (r + 2) / 2 <= n) {
        ++r;
    }

    /* Column within that row. */
    idx_t offset = r * (r + 1) / 2;
    idx_t k = n - offset;

    /* Compute rows up to r if necessary. */
    ensure_rows(r);

    return tri_rows[(size_t)r][(size_t)k];
}
