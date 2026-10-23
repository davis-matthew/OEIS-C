/*
 * A159881  Triangle read by rows : T(n,0) = n+1, T(n,k)=0 if k<0 or if k>n,
 *          T(n,k) = k*T(n-1,k) - T(n-1,k-1).
 * https://oeis.org/A159881
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign, tabl
 * Formula:   not stated
 * Range:     i8 n <= 23, i16 n <= 58, i32 n <= 141, i64 n <= 359,
 *            i128 n <= 1001, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence:  T(n,0)=n+1;  T(n,k)=k*T(n-1,k)-T(n-1,k-1)  (k>0);
   a(n) is the triangle read by rows: a = [T(0,0),T(1,0),T(1,1),T(2,0),T(2,1),T(2,2), …] */
#include <stdint.h>
#include <stdlib.h>

/*--- helper: find the row containing the flattened index n ----------------*/
static idx_t
row_for_index(idx_t n)
{
    /* binary search on the triangular numbers r*(r+1)/2 <= n */
    idx_t lo = 0, hi = 1;
    for (;; hi <<= 1) {
        __int128 prod = (__int128)hi * (hi + 1) / 2;
        if (prod > (__int128)n)
            break;
    }
    for (;;) {
        if (lo + 1 == hi)
            return lo;
        idx_t mid = lo + (hi - lo) / 2;
        __int128 prod = (__int128)mid * (mid + 1) / 2;
        if (prod <= (__int128)n)
            lo = mid;
        else
            hi = mid;
    }
}

/*--- main sequence routine --------------------------------------------------*/
seq_t a(idx_t n)
{
    /* map flattened index n to (row, col) coordinates in the triangle */
    idx_t row = row_for_index(n);                     /* 0‑based row index            */
    __int128 pref = (__int128)row * (row + 1) / 2;    /* elements before this row     */
    idx_t col = n - (idx_t)pref;                      /* column within the row        */

    /* static cache of computed rows */
    static seq_t **triangle = NULL;     /* array of row pointers         */
    static size_t rows_cached = 0;      /* number of rows currently stored*/

    /* enlarge the cache if needed */
    if ((size_t)row >= rows_cached) {
        size_t needed = (size_t)row + 1;

        /* grow the array of row pointers */
        triangle = (seq_t**)realloc(triangle, needed * sizeof(seq_t*));

        /* compute new rows only once */
        for (size_t i = rows_cached; i < needed; ++i) {
            seq_t *row_arr = (seq_t*)malloc((i + 1) * sizeof(seq_t));
            if (!row_arr)
                abort();   /* allocation failure */

            row_arr[0] = (seq_t)(i + 1);   /* T(i,0) = i+1 */
            for (size_t k = 1; k <= i; ++k) {
                seq_t prev_k = (k <= i - 1) ? triangle[i - 1][k] : (seq_t)0;
                row_arr[k] = (seq_t)k * prev_k - triangle[i - 1][k - 1];
            }
            triangle[i] = row_arr;
        }
        rows_cached = needed;
    }

    return triangle[row][col];
}
