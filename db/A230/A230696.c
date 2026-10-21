/*
 * A230696  Triangle read by rows related to double factorial of odd numbers
 *          (A001147).
 * https://oeis.org/A230696
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 15, u16 n <= 28, u32 n <= 44, u64 n <= 44, u128 n <= 44,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A230696 – Flattened triangle T(n,k) with
     T(n,k) = 2*T(n-1,k-1) + ((2n-3)^2)*T(n-2,k)
     T(0,0)=1,  T(1,0)=0,  T(1,1)=1,  T(n,k)=0 for k>n or k<0.
   The sequence a(n) is obtained by reading the triangle row-by-row.
   Initial terms: a(0)=1, a(1)=0, a(2)=1, a(3)=1, a(4)=0, … */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* -----  Memory helpers for seq_t objects  ----- */
static inline seq_t *alloc_seq_array(size_t len)
{
    /* Allocate len elements of type seq_t, zero-initialised */
    return (seq_t *)calloc(len, sizeof(seq_t));
}

/* -----  Triangle storage  ----- */
static seq_t **tri_rows = NULL;        /* array of pointers to each row    */
static size_t tri_rows_len = 0;        /* number of rows currently stored  */
static size_t tri_rows_cap = 0;        /* current capacity of tri_rows array */

/* Ensure that rows 0 … upto (inclusive) are available. */
static void ensure_tri_rows(size_t upto)
{
    if (tri_rows_len > upto)
        return;

    /* Grow the container of row pointers if necessary. */
    if (tri_rows_cap == 0) {
        tri_rows_cap = 1;
        tri_rows = (seq_t **)malloc(sizeof(seq_t *));
    } else if (tri_rows_len >= tri_rows_cap) {
        tri_rows_cap *= 2;
        tri_rows = (seq_t **)realloc(tri_rows, tri_rows_cap * sizeof(seq_t *));
    }
    if (!tri_rows) abort();  /* allocation failure */

    /* Build missing rows. */
    for (size_t r = tri_rows_len; r <= upto; ++r) {
        seq_t *row = alloc_seq_array(r + 1);
        if (!row) abort();

        if (r == 0) {
            row[0] = (seq_t)1;
        } else if (r == 1) {
            row[0] = (seq_t)0;
            row[1] = (seq_t)1;
        } else {
            /* ((2r-3)^2) */
            seq_t coeff = (seq_t)(2 * r - 3);
            coeff = coeff * coeff;

            for (size_t k = 0; k <= r; ++k) {
                seq_t term1 = 0;
                seq_t term2 = 0;
                if (k >= 1)
                    term1 = tri_rows[r - 1][k - 1];
                if (k <= r - 2)
                    term2 = tri_rows[r - 2][k];
                row[k] = term1 * (seq_t)2 + term2 * coeff;
            }
        }
        tri_rows[r] = row;
        tri_rows_len = r + 1;
    }
}

/* Return the n‑th term of A230696. */
seq_t a(idx_t n)
{
    /* Determine the row r containing index n:
       indices for row r range from r*(r+1)/2  to  (r+1)*(r+2)/2 – 1. */
    size_t r = 0;
    while ((unsigned __int128)(r + 1) * (r + 2) / 2 <= n)
        ++r;

    size_t row_start = r * (r + 1) / 2;
    size_t k = (size_t)n - row_start;   /* column within the row */

    ensure_tri_rows(r);
    return tri_rows[r][k];
}
