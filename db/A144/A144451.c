/*
 * A144451  Triangle A(n,k) read by rows: A(n,1)=A(n,n)=1 and A(n,k) =
 *          A(n-2,k-1) - 2*A(n,k-1) + 1 for 1<k<n.
 * https://oeis.org/A144451
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  tabl, sign, easy
 * Formula:   not stated
 * Range:     i8 n <= 55, i16 n <= 55, i32 n <= 55, i64 n <= 55, i128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Triangle A144451:
   A(n,1)=A(n,n)=1
   A(n,k)=A(n-2,k-1)-2*A(n,k-1)+1   (1<k<n)          */
/* The sequence a(n) is obtained by reading the triangle row‑by‑row,
   so a(n)=A(r,c) with r*(r-1)/2 < n <= r*(r+1)/2,  c = n - r*(r-1)/2.   */
#include <stdint.h>
#include <stdlib.h>
#include <string.h>   /* for memcpy */

seq_t a(idx_t n)
{
    /* static storage for the triangle values, linearised by rows */
    static seq_t *tri = NULL;          /* array of all A(r,c) values  */
    static idx_t capacity = 0;         /* current allocated slots      */
    static idx_t max_row = 0;          /* highest row that has been built */

    /* --- find the row that contains the n-th element --- */
    idx_t r = 1;
    while ( ((seq_t)r * ((seq_t)r + 1)) / 2 < n )
        ++r;

    /* --- grow storage if necessary and build new rows --- */
    if (r > max_row) {
        /* capacity needed for rows up to r */
        idx_t new_cap = (r * (r + 1)) / 2;

        if (new_cap > capacity) {
            seq_t *new_tri;
#ifdef __cplusplus
            new_tri = new seq_t[new_cap];
            /* copy existing elements */
            for (idx_t i = 0; i < capacity; ++i)
                new_tri[i] = tri[i];
            /* delete old array */
            delete[] tri;
#else
            new_tri = (seq_t*)malloc(new_cap * sizeof(seq_t));
            if (new_tri == NULL)
                return 0;   /* out‑of‑memory; fallback to 0 */
            /* copy existing elements */
            memcpy(new_tri, tri, capacity * sizeof(seq_t));
            free(tri);
#endif
            tri = new_tri;
            capacity = new_cap;
        }

        /* build rows from (max_row+1) up to r */
        for (idx_t row = max_row + 1; row <= r; ++row) {
            idx_t start = (row * (row - 1)) / 2;   /* index of A(row,1) */

            if (row <= 2) {          /* rows 1 and 2 are all ones */
                for (idx_t c = 0; c < row; ++c)
                    tri[start + c] = (seq_t)1;
            } else {
                tri[start] = (seq_t)1;                 /* first element */

                /* intermediate elements 2 .. row-1 */
                for (idx_t c = 2; c < row; ++c) {
                    idx_t idx_curr  = start + (c - 1);                  /* A(row,c)  */
                    idx_t idx_prev  = start + (c - 2);                  /* A(row,c-1) */
                    idx_t idx_left  = ((row - 2) * (row - 3)) / 2 + (c - 2); /* A(row-2,c-1) */
                    tri[idx_curr] = tri[idx_left] - (seq_t)2 * tri[idx_prev] + (seq_t)1;
                }

                tri[start + row - 1] = (seq_t)1;  /* last element */
            }
        }
        max_row = r;
    }

    return tri[n - 1];
}
