/*
 * A133366  Triangle T(n,k)read by rows given by [3,1,3,1,3,1,3,1,3,1,3,1,...]
 *          DELTA [1,0,0,0,0,0,0,0,...] where DELTA is the operator defined in
 *          A084938.
 * https://oeis.org/A133366
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) is the triangle T read by rows. T(0,0)=1;
 *            T(n,0)=3*T(n-1,0)+3*T(n-1,1);
 *            T(n,k)=T(n-1,k-1)+4*T(n-1,k)+3*T(n-1,k+1) for k≥1. a(n)=T(r,k)
 *            where r satisfies r*(r+1)/2 ≤ n < (r+1)*(r+2)/2 and k = n -
 *            r*(r+1)/2
 * Range:     u8 n <= 9, u16 n <= 35, u32 n <= 44, u64 n <= 44, u128 n <= 44,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    static seq_t **rows = NULL;   /* stored rows of the triangle */
    static size_t rows_cap = 0;
    static idx_t maxRow = (idx_t)-1;  /* no rows computed yet */

    /* initialize the first row if needed */
    if (maxRow == (idx_t)-1) {
        rows_cap = 1;
        rows = (seq_t**)malloc(rows_cap * sizeof(*rows));
        rows[0] = (seq_t*)malloc(sizeof(seq_t) * 1);
        rows[0][0] = (seq_t)1;
        maxRow = 0;
    }

    /* find the row r and column k for the given index n */
    idx_t r = 0;
    while (((r + 1) * (r + 2)) / 2 <= n) ++r;
    idx_t start = r * (r + 1) / 2;
    idx_t k = n - start;

    /* compute rows up to r if they don't exist yet */
    if (r > maxRow) {
        /* enlarge the rows array if necessary */
        if ((size_t)(r + 1) > rows_cap) {
            size_t new_cap = rows_cap ? rows_cap : 1;
            while (new_cap <= (size_t)(r + 1)) new_cap <<= 1;
            seq_t **tmp = (seq_t**)realloc(rows, new_cap * sizeof(*tmp));
            rows = tmp;
            rows_cap = new_cap;
        }
        for (idx_t i = maxRow + 1; i <= r; ++i) {
            idx_t len = i + 1;
            seq_t *curr = (seq_t*)malloc(sizeof(seq_t) * (size_t)len);
            seq_t *prev = rows[i - 1];

            /* first element: T(i,0) */
            seq_t prev0 = prev[0];
            seq_t prev1 = (i >= 2) ? prev[1] : (seq_t)0;
            curr[0] = prev0 * (seq_t)3 + prev1 * (seq_t)3;

            /* middle elements: T(i,j) for 1 <= j < i */
            for (idx_t j = 1; j < i; ++j) {
                seq_t a = prev[j - 1];
                seq_t b = prev[j];
                seq_t c = (j + 1 < i) ? prev[j + 1] : (seq_t)0;
                curr[j] = a + b * (seq_t)4 + c * (seq_t)3;
            }

            /* last element: T(i,i) = T(i-1,i-1) */
            curr[i] = prev[i - 1];

            rows[i] = curr;
        }
        maxRow = r;
    }

    return rows[r][k];
}
