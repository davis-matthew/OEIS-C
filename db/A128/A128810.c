/*
 * A128810  Triangle formed by reading triangle A064189 mod 2 .
 * https://oeis.org/A128810
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 104, u16 n <= 104, u32 n <= 104, u64 n <= 104,
 *            u128 n <= 104, big any n
 * Helpers:   alloc_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Triangle A064189 mod 2, read row‑wise. The element a(n) is the
   term T(r,k) with r the unique integer satisfying
   r*(r+1)/2 ≤ n < (r+1)*(r+2)/2, and k = n - r*(r+1)/2. */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/alloc_seq.h"     /* alloc_seq */

seq_t a(idx_t n)
{
    /* cache of rows of the triangle */
    static seq_t **rows = NULL;          /* pointer to each row array */
    static size_t rows_cap = 0;          /* allocated capacity in rows */
    static size_t rows_used = 0;         /* number of rows currently stored */

    /* find row r such that r*(r+1)/2 ≤ n < (r+1)*(r+2)/2 */
    idx_t r = 0;
    while ((r + 1) * (r + 2) / 2 <= n)
        ++r;

    /* ensure that rows 0 … r are present */
    if (rows_used <= (size_t)r) {
        /* grow the array of row pointers if necessary */
        size_t new_cap = rows_used ? rows_used * 2 : 1;
        while (new_cap <= (size_t)r)
            new_cap = new_cap ? new_cap * 2 : 1;
        if (new_cap > rows_cap) {
            seq_t **tmp = (seq_t**)realloc(rows, new_cap * sizeof(seq_t*));
            if (!tmp && new_cap) return 0;          /* allocation failed */
            rows = tmp;
            rows_cap = new_cap;
        }

        /* generate missing rows */
        for (idx_t i = (idx_t)rows_used; i <= r; ++i) {
            size_t len = (size_t)(i + 1);
            seq_t *row = alloc_seq(len);           /* array of len seq_t */
            if (!row) return 0;                    /* allocation failed */

            if (i == 0) {
                row[0] = 1;                        /* T(0,0) = 1 */
            } else {
                seq_t *prev = rows[i - 1];
                for (size_t k = 0; k < len; ++k) {
                    seq_t val = 0;
                    if (k > 0)          val += prev[k - 1];
                    if (k < i)          val += prev[k];
                    if (k + 1 < i)       val += prev[k + 1];
                    row[k] = val & (seq_t)1;           /* mod 2 */
                }
            }
            rows[i] = row;
        }
        rows_used = (size_t)r + 1;                     /* rows 0…r exist */
    }

    /* index of first element in row r */
    idx_t start = r * (r + 1) / 2;
    idx_t k = n - start;                              /* column within row r */
    return rows[r][k];
}
