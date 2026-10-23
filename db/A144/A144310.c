/*
 * A144310  Triangle read by rows: prime numbers p(n) along left edge (n, 1)
 *          and p(n - 1) along right edge (n, n), with (n, k) = rad((n - 1, k
 *          - 1) + (n - 1, k)), multiplied by two if four divides (n - 1, k -
 *          1) + (n - 1, k), for 1 < k < n when n > 2.
 * https://oeis.org/A144310
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, tabl
 * Formula:   Triangle read by rows where T(n,k) = rad( T(n-1,k-1) + T(n-1,k)
 *            ) for 1 < k < n, with edges T(n,1)=p(n) (nth prime) and
 *            T(n,n)=p(n-1). The sequence a(n) is obtained by reading the
 *            triangle row by row. For the interior entries, if the sum is
 *            divisible by 4 the result is multiplied by 2
 * Range:     u8 n <= 60, u16 n <= 66, u32 n <= 66, u64 n <= 66, u128 n <= 66,
 *            big any n
 * Helpers:   get_prime, rad
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Include required utilities */
#include <stdlib.h>
#include <stdint.h>
#include "../utils/rad.h"
#include "../utils/get_prime.h"

static seq_t **triangle_tbl = NULL;   /* array of pointers to each row */
static size_t  rows_count  = 0;        /* how many rows have been built */
static size_t  tbl_cap     = 0;        /* capacity of triangle_tbl */

/* Build the next row (with index cur_row starting at 1) */
static void build_next_row(void)
{
    size_t cur_row = rows_count + 1;                 /* 1‑based row index */
    size_t len     = cur_row;                       /* number of columns */
    seq_t  *row    = (seq_t*)malloc(len * sizeof(seq_t));
    if (!row) abort();                               /* allocation failure */

    for (size_t col = 0; col < len; ++col) {
        seq_t val;
        if (col == 0) {                               /* left edge */
            val = (seq_t)get_prime(cur_row);
        } else if (col == len - 1) {                  /* right edge */
            val = (seq_t)get_prime(cur_row - 1);
        } else {                                      /* interior */
            seq_t sum = triangle_tbl[rows_count-1][col-1] + triangle_tbl[rows_count-1][col];
            seq_t rad_val = rad((idx_t)sum);
            if ((sum & 3) == 0) rad_val *= 2;          /* multiply by 2 if 4 divides the sum */
            val = rad_val;
        }
        row[col] = val;
    }

    /* Grow the triangle table if necessary */
    if (rows_count >= tbl_cap) {
        tbl_cap = (tbl_cap == 0) ? 8 : tbl_cap << 1;
        triangle_tbl = (seq_t**)realloc(triangle_tbl, tbl_cap * sizeof(seq_t*));
        if (!triangle_tbl) abort();
    }

    triangle_tbl[rows_count++] = row;
}

/* Return value at triangular index (row,r,col) for requested n */
seq_t a(idx_t n)
{
    if (n < 1) return 0;  /* undefined for n<1, return 0 */

    /* Determine row r such that r*(r+1)/2 >= n */
    size_t r = 1;
    while ((idx_t)r * (r + 1) / 2 < n) ++r;
    size_t prev_tri = (r - 1) * r / 2;  /* number of entries before row r */
    size_t c = n - prev_tri;           /* 1‑based column in row r */

    /* Build rows up to r if not already present */
    while (rows_count < r) build_next_row();

    /* Return the desired entry */
    return triangle_tbl[r-1][c-1];
}
