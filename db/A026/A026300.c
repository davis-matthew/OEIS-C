/*
 * A026300  Motzkin triangle, T, read by rows; T(0,0) = T(1,0) = T(1,1) = 1;
 *          for n >= 2, T(n,0) = 1, T(n,k) = T(n-1,k-2) + T(n-1,k-1) +
 *          T(n-1,k) for k = 1,2,...,n-1 and T(n,n) = T(n-1,n-2) + T(n-1,n-1).
 * https://oeis.org/A026300
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, nice
 * Formula:   not stated
 * Range:     u8 n <= 40, u16 n <= 100, u32 n <= 317, u64 n <= 1069,
 *            u128 n <= 3811, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: a(n) is the n-th element of the Motzkin triangle read in row‑major order.
 * Recurrence for T(n,k):
 *   T(0,0)=1;      T(1,0)=T(1,1)=1;
 *   for n>=2:
 *     T(n,0)=1;
 *     T(n,k)=T(n-1,k-2)+T(n-1,k-1)+T(n-1,k)   (1 ≤ k ≤ n-1)
 *     T(n,n)=T(n-1,n-2)+T(n-1,n-1).
 * Then a(n)=T(r,c) where
 *   r = floor((sqrt(8*n+1)-1)/2)   (row number)
 *   c = n - r*(r+1)/2              (column offset)
 */

#include <stdlib.h>
#include <math.h>

seq_t a(idx_t n)
{
    /* static cache of computed rows */
    static seq_t **rows = NULL;
    static size_t rows_count = 0;

    /* find row r such that r*(r+1)/2 ≤ n < (r+1)*(r+2)/2 */
    idx_t r = (idx_t)((sqrt((double)8 * n + 1.0) - 1.0) * 0.5);
    /* adjust if rounding errors occurred */
    while (r * (r + 1) / 2 > n) --r;
    while ((r + 1) * (r + 2) / 2 <= n) ++r;

    /* offset inside that row */
    idx_t prefix = r * (r + 1) / 2;
    idx_t c = n - prefix;

    /* ensure rows up to r are computed */
    if (rows_count <= r) {
        size_t new_count = (size_t)r + 1;
        seq_t **tmp = (seq_t **)realloc(rows, new_count * sizeof(seq_t *));
        rows = tmp;
        for (size_t i = rows_count; i < new_count; ++i) {
            rows[i] = (seq_t *)calloc((size_t)i + 1, sizeof(seq_t));
            /* T(i,0) = 1  and (when i>=1) T(i,i) defined below */
            rows[i][0] = 1;
            if (i >= 1) {
                /* middle elements k = 1 .. i-1 */
                for (size_t k = 1; k < i; ++k) {
                    seq_t sum = 0;
                    if (k >= 2) sum += rows[i - 1][k - 2];
                    sum += rows[i - 1][k - 1];
                    sum += rows[i - 1][k];
                    rows[i][k] = sum;
                }
                /* last element k = i */
                seq_t sum = rows[i - 1][i - 1];
                if (i >= 2) sum += rows[i - 1][i - 2];
                rows[i][i] = sum;
            }
        }
        rows_count = new_count;
    }

    return rows[r][c];
}
