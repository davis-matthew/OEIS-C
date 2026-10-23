/*
 * A030526  A convolution triangle of numbers obtained from A036070.
 * https://oeis.org/A030526
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 15, u32 n <= 45, u64 n <= 45, u128 n <= 45,
 *            big any n
 * Helpers:   array_delete
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A030526: convolution triangle of numbers obtained from A036070
   Recurrence (OEIS definition):
      a(n,m) = (4*(4*m+n-1)*a(n-1,m) + m*a(n-1,m-1)) / n
      with a(1,1)=1, a(n,0)=a(n,m)=0 for n<m
   The one‑dimensional sequence is obtained by reading the triangle
   row by row.  For n >= 1 a(n) = a(row, col) where
      row = smallest r with r(r+1)/2 >= n
      col = n - (r-1)r/2
*/

#include <stdint.h>
#include <stdlib.h>
#include "../utils/array_delete.h"

/* allocate an array of seq_t that works in C and C++ */
static seq_t* array_new(size_t n)
{
#ifdef __cplusplus
    return new seq_t[n];
#else
    return (seq_t*)malloc(n * sizeof(seq_t));
#endif
}

/* triangular number T(r) = r*(r+1)/2 */
static idx_t triangular(idx_t r)
{
    return r * (r + 1) / 2;
}

/* flattened triangle storage and state */
static seq_t* flat = NULL;      /* contiguous storage of a(row,col) values */
static idx_t cur_tot = 0;       /* number of values currently stored (triangular(cur_r)) */
static idx_t cur_r   = 0;       /* highest row that has been fully computed */

/* ensure flat has space for at least new_tot elements */
static void grow_flat(idx_t new_tot)
{
    seq_t* new_flat = array_new(new_tot);
    for (idx_t i = 0; i < cur_tot; ++i)
        new_flat[i] = flat[i];
    if (flat)
        array_delete(flat);
    flat = new_flat;
    cur_tot = new_tot;
}

/* build rows r = start_row .. end_row (inclusive) */
static void build_rows(idx_t start_row, idx_t end_row)
{
    /* required storage */
    idx_t need = triangular(end_row);
    if (need > cur_tot)
        grow_flat(need);

    for (idx_t r = start_row; r <= end_row; ++r) {
        idx_t prev_start = (r >= 2) ? triangular(r - 2) : 0;       /* start of row r-1 */
        idx_t row_start  = triangular(r - 1);                       /* start of row r */

        for (idx_t m = 1; m <= r; ++m) {
            seq_t val;
            if (r == 1 && m == 1) {
                val = 1;   /* a(1,1) = 1 */
            } else {
                seq_t a_nm   = 0;   /* a(r-1, m) */
                seq_t a_n1m1 = 0;   /* a(r-1, m-1) */

                if (m <= r - 1)                      /* a(r-1, m) exists */
                    a_nm = flat[prev_start + (m - 1)];
                if (m >= 2)                          /* a(r-1, m-1) exists */
                    a_n1m1 = flat[prev_start + (m - 2)];

                seq_t term1 = a_nm * (4 * ((4 * m + r - 1))); /* numerator part1 */
                seq_t term2 = a_n1m1 * m;                     /* numerator part2 */
                val = (term1 + term2) / r;                   /* exact division */
            }
            flat[row_start + (m - 1)] = val;
        }
    }
    cur_r = end_row;
}

/* public function */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;   /* undefined below first term, but harmless */

    /* locate row containing n */
    idx_t r = cur_r;
    while (triangular(r) < n)
        ++r;

    if (r > cur_r)
        build_rows(cur_r + 1, r);

    return flat[n - 1];
}
