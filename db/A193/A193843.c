/*
 * A193843  Mirror image of the triangle A193842.
 * https://oeis.org/A193843
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   a(n) is obtained by scanning the triangle A193843. The triangle
 *            satisfies T(n,0) = (3^(n+1) - 1)/2 T(n,k) = 3*T(n-1,k) +
 *            T(n-1,k-1) for k ≥ 1 where T(n,k) is the term at row n and
 *            column k (0‑based). The sequence a(n) is the linear indexing of
 *            this triangle row by row
 * Range:     u8 n <= 14, u16 n <= 45, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t N)
{
    /* Linear index of the sequence element. */
    idx_t idx = N;

    /* Find the row r such that r*(r+1)/2 ≤ idx < (r+1)*(r+2)/2. */
    idx_t low = 0, high = idx, r = 0;
    while (low <= high) {
        idx_t mid = (low + high) >> 1;
        idx_t prefix = mid * (mid + 1) / 2;
        if (prefix <= idx) {
            r = mid;
            low = mid + 1;
        } else {
            high = mid - 1;
        }
    }

    /* Column within that row. */
    idx_t k = idx - r * (r + 1) / 2;

    /* Static cache of previously computed rows of the triangle. */
    static seq_t **triangle = NULL;
    static idx_t cached_rows = 0;       /* number of rows currently cached */
    static seq_t pow3 = 1;              /* == 3^(row+1) for the last cached row */

    /* Ensure we have computed rows up to r. */
    for (idx_t cur = cached_rows; cur <= r; ++cur) {
        /* Resize array of row pointers. */
        triangle = (seq_t **)realloc(triangle, (cur + 2) * sizeof(seq_t *));
        /* Allocate the current row (length cur+1). */
        triangle[cur] = (seq_t *)malloc((cur + 1) * sizeof(seq_t));

        /* Update power of 3 for this row. */
        pow3 *= 3;                     /* pow3 now holds 3^(cur+1) */

        /* Compute T(cur,0). */
        triangle[cur][0] = (pow3 - 1) / 2;

        /* Compute remaining entries using the recurrence. */
        if (cur > 0) {
            seq_t *prev = triangle[cur - 1];
            for (idx_t c = 1; c <= cur; ++c) {
                seq_t val = 0;
                if (c <= cur - 1)
                    val += 3 * prev[c];
                val += prev[c - 1];
                triangle[cur][c] = val;
            }
        }
    }
    cached_rows = r + 1;

    return triangle[r][k];
}
