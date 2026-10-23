/*
 * A147724  a(n) = C(3,n) DELTA C(0,n).
 * https://oeis.org/A147724
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 14, u16 n <= 28, u32 n <= 90, u64 n <= 300,
 *            u128 n <= 1082, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A147724:  a(n) = T(r,c)
   T is the Deléham array defined by
     T(0,0)=1,
     T(1,0)=T(1,1)=1,
     T(2,0)=4, T(2,1)=5, T(2,2)=1
   for n≥3, 0≤k≤n
     T(n,k)=8*T(n-1,k)+T(n-1,k-1)-7*T(n-2,k)-4*T(n-2,k-1)
   The sequence a(n) is obtained by reading T row‑by‑row
   (row 0, then row 1, row 2, …).  For a given index n we first
   find the unique row r such that r(r+1)/2 ≤ n < (r+1)(r+2)/2,
   then col = n − r(r+1)/2 and a(n)=T(r,col).  A memoised
   construction of the triangle guarantees O(n) time per call. */

#include <stdint.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* locate the row r containing index n */
    idx_t r = 0;
    while ((r + 1) * (r + 2) / 2 <= n)
        ++r;
    idx_t col = n - r * (r + 1) / 2;    /* 0 ≤ col ≤ r */

    /* dynamic array of pointers to the rows already built */
    static seq_t **row_ptr  = NULL;
    static idx_t  rows_used = 0;
    static size_t rows_cap  = 0;

    /* initialise the first three rows on the first call */
    if (rows_used == 0) {
        rows_cap = 3;
        row_ptr  = (seq_t **)malloc(rows_cap * sizeof(seq_t *));
        if (!row_ptr) return (seq_t)0;           /* allocation error */

        /* row 0 : 1 */
        row_ptr[0] = (seq_t *)malloc(1 * sizeof(seq_t));
        if (!row_ptr[0]) return (seq_t)0;
        row_ptr[0][0] = (seq_t)1;

        /* row 1 : 1 , 1 */
        row_ptr[1] = (seq_t *)malloc(2 * sizeof(seq_t));
        if (!row_ptr[1]) return (seq_t)0;
        row_ptr[1][0] = (seq_t)1;
        row_ptr[1][1] = (seq_t)1;

        /* row 2 : 4 , 5 , 1 */
        row_ptr[2] = (seq_t *)malloc(3 * sizeof(seq_t));
        if (!row_ptr[2]) return (seq_t)0;
        row_ptr[2][0] = (seq_t)4;
        row_ptr[2][1] = (seq_t)5;
        row_ptr[2][2] = (seq_t)1;

        rows_used = 3;
    }

    /* grow the row pointer array if necessary */
    while (rows_cap <= r) {
        size_t new_cap = rows_cap * 2;
        if (new_cap == 0) new_cap = r + 1;
        seq_t **tmp = (seq_t **)realloc(row_ptr, new_cap * sizeof(seq_t *));
        if (!tmp) return (seq_t)0;
        row_ptr = tmp;
        rows_cap = new_cap;
    }

    /* compute all missing rows up to r */
    for (idx_t i = rows_used; i <= r; ++i) {
        seq_t *row = (seq_t *)malloc((i + 1) * sizeof(seq_t));
        if (!row) return (seq_t)0;
        for (idx_t k = 0; k <= i; ++k) {
            seq_t value = 0;
            if (i == 0) {                /* row 0 */
                value = (seq_t)1;
            } else if (i == 1) {         /* row 1 */
                value = (seq_t)1;
            } else if (i == 2) {         /* row 2 */
                if (k == 0)      value = (seq_t)4;
                else if (k == 1) value = (seq_t)5;
                else             value = (seq_t)1;
            } else {                      /* recurrence for i ≥ 3 */
                seq_t t1 = 0, t2 = 0, t3 = 0, t4 = 0;
                if (k <= i - 1) t1 = (seq_t)(8 * row_ptr[i - 1][k]);
                if (k >= 1)      t2 = row_ptr[i - 1][k - 1];
                if (k <= i - 2) t3 = (seq_t)(7 * row_ptr[i - 2][k]);
                if (k >= 1 && k - 1 <= i - 2)
                    t4 = (seq_t)(4 * row_ptr[i - 2][k - 1]);
                value = t1 + t2 - t3 - t4;
            }
            row[k] = value;
        }
        row_ptr[i] = row;
        ++rows_used;
    }

    return row_ptr[r][col];
}
