/*
 * A072024  Table by antidiagonals of T(n,k) = ((n+1)^k - (-n)^k)/(2*n+1).
 * https://oeis.org/A072024
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 46, u16 n <= 93, u32 n <= 194, u64 n <= 471,
 *            u128 n <= 1186, big any n
 * Helpers:   T_row_k
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The sequence A072024:  a(n) is the entry in a table built on the
   formula                                              
        T(n,k) = ((n+1)^k - (-n)^k)/(2n+1)             
   where the tuple (n,k) is obtained by reading the table
   by antidiagonals (offset 0).  The recurrence
        T(n,0)=0 ,  T(n,1)=1 ,
        T(n,k)=T(n,k-1)+n(n+1)T(n,k-2)   (k≥2)
   is used so that no 64‑bit overflows occur when the result
   still fits in a 64‑bit word.  For arbitrary‑precision builds
   the same recurrence gives the exact result. */

#include <stdint.h>
#include "../utils/T_row_k.h"

/* map the linear index n to the pair (row,col) obtained by
   reading the table by antidiagonals.  */
static void index_to_row_col(idx_t n, idx_t *row, idx_t *col)
{
    /* find d such that d(d+1)/2 <= n < (d+1)(d+2)/2 */
    idx_t d = 0;
    while ((unsigned __int128)(d + 1) * (d + 2) / 2 <= n)
        ++d;

    *row = n - d * (d + 1) / 2;
    *col = d - *row;
}

/* main sequence function */
seq_t a(idx_t n)
{
    idx_t row, col;
    index_to_row_col(n, &row, &col);
    return T_row_k(row, col);
}
