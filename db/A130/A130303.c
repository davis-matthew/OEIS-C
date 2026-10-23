/*
 * A130303  A130296 * A000012.
 * https://oeis.org/A130303
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 55, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula:  a(n) = e(r,k) where r is the row such that r(r+1)/2 >= n,
 *             k = n - r(r-1)/2.
 *   The triangular array e(r,k) is defined by
 *     e(r,1) = 2r-1,
 *     e(r,r) = 1,
 *     e(r,k) = (e(r-1,k)*e(r,k-1)+1)/e(r-1,k-1)   for 1 < k < r.
 *   a(1)=1, a(2)=3, a(3)=1, a(4)=5, a(5)=2, a(6)=1, ...
 */

#include <stdlib.h>

/* seq_t and idx_t are supplied by the build system. */
seq_t a(idx_t n)
{
    /* static storage for already computed rows */
    static seq_t **rows = NULL;      /* rows[1..max_row] */
    static idx_t  max_row = 0;       /* highest row computed so far */

    if (n == 0)
        return (seq_t)0;   /* offset is 1; behaviour for n=0 is undefined */

    /* Find the row r such that r(r+1)/2 >= n */
    idx_t r = 1;
    idx_t cum = 1;              /* cumulative elements up to current row */
    while (cum < n) {
        ++r;
        cum += r;
    }

    /* Grow the storage to contain rows up to r */
    if (max_row < r) {
        /* Allocate (or reallocate) the array of row pointers */
        size_t new_cap = (size_t)(r + 1);     /* indices 0..r, 0 unused */
        if (rows == NULL) {
            rows = (seq_t**)malloc(new_cap * sizeof(seq_t*));
        } else if (new_cap > (size_t)(max_row + 1)) {
            rows = (seq_t**)realloc(rows, new_cap * sizeof(seq_t*));
        }

        /* Compute each new row */
        for (idx_t cur = max_row + 1; cur <= r; ++cur) {
            seq_t *row = (seq_t*)malloc((size_t)(cur + 1) * sizeof(seq_t));
            /* first element (odd number) */
            row[1] = (seq_t)2 * cur - 1;
            /* last element is always 1 */
            row[cur] = (seq_t)1;

            if (cur > 1) {
                seq_t *prev = rows[cur - 1];
                /* compute inside entries k = 2 .. cur-1 */
                for (idx_t k = 2; k < cur; ++k) {
                    /* (e(cur-1,k) * e(cur,k-1) + 1) / e(cur-1,k-1) */
                    seq_t num = prev[k] * row[k - 1] + (seq_t)1;
                    seq_t den = prev[k - 1];
                    row[k] = num / den;
                }
            }
            rows[cur] = row;
        }
        max_row = r;
    }

    /* determine column index k within row r */
    idx_t prev_total = (r - 1) * r / 2;   /* sum of 1..(r-1) */
    idx_t k = n - prev_total;            /* 1 <= k <= r */

    return rows[r][k];
}
