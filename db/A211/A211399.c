/*
 * A211399  Triangle T(n,k), 0 <= k <= n, given by (0, 1, 0, 2, 0, 3, 0, 4, 0,
 *          5, 0, ...) DELTA (1, 0, 3, 0, 5, 0, 7, 0, 9, 0, ...) where DELTA
 *          is the operator defined in A084938.
 * https://oeis.org/A211399
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 23, u16 n <= 40, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A211399 – flattened triangle T(n,k)
 *  Recurrence:  T(n,k) = k*T(n-1,k) + (2*n-2*k+1)*T(n-1,k-1)
 *  with  T(n,n)=1 and T(n,k)=0 for k<0 or k>n.
 *  a(n) returns the entry T(r,k) with
 *      r = floor((sqrt(8*n+1)-1)/2)
 *      k = n - r*(r+1)/2
 *  The implementation builds the triangle on demand, caching all rows
 *  it has already computed.  Only integer arithmetic is used, so it
 *  works with any integer type supplied by the build system.
 */

#include <stdlib.h>
#include <math.h>

seq_t a(idx_t n)
{
    /* Cached (flattened) triangle */
    static seq_t *triangle = NULL;
    static idx_t max_row = 0;      /* last fully computed row (0‑based) */
    static size_t allocated = 0;   /* number of entries allocated    */

    /* Locate the (r,k) pair that corresponds to the flat index n */
    double d = (sqrt(8.0 * (double)n + 1.0) - 1.0) * 0.5;
    idx_t r = (idx_t)d;                                            /* r >= 0   */
    idx_t row_start = r * (r + 1) / 2;                              /* first index of row r */
    idx_t k = n - row_start;                                       /* 0 <= k <= r */

    /* Expand the cached triangle if we need more rows */
    if (r >= max_row) {
        idx_t new_max_row = r + 1; /* rows 0 .. r inclusive          */
        size_t new_size = new_max_row * (new_max_row + 1) / 2;     /* total entries needed */

        /* realloc can handle a NULL pointer */
        seq_t *new_tri = (seq_t *)realloc(triangle, new_size * sizeof(seq_t));
        if (!new_tri) {
            /* Allocation failed – program must terminate elsewhere */
            return (seq_t)0;
        }
        triangle = new_tri;

        /* Compute all new rows from max_row up to r */
        for (idx_t i = max_row; i < new_max_row; ++i) {
            idx_t i_start = i * (i + 1) / 2;   /* index of T(i,0)          */
            idx_t prev_start = (i - 1) * i / 2;/* index of T(i-1,0)        */
            for (idx_t j = 0; j <= i; ++j) {
                if (j == i) {                /* diagonal element */
                    triangle[i_start + j] = (seq_t)1;
                } else {
                    seq_t value = 0;
                    /* k * T(i-1,k) – only defined if k <= i-1 */
                    if (j <= i - 1) {
                        value += triangle[prev_start + j] * (seq_t)j;
                    }
                    /* (2*i-2*j+1) * T(i-1,j-1) – only if j>0 */
                    if (j > 0) {
                        value += triangle[prev_start + (j - 1)]
                                 * (seq_t)(2 * i - 2 * j + 1);
                    }
                    triangle[i_start + j] = value;
                }
            }
        }
        allocated = new_size;
        max_row = new_max_row;
    }

    return triangle[row_start + k];
}
