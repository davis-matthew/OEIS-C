/*
 * A185422  Forests of k increasing plane unary-binary trees on n nodes.
 *          Generalized Stirling numbers of the second kind associated with
 *          A185415.
 * https://oeis.org/A185422
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, tabl
 * Formula:   not stated
 * Range:     u8 n <= 16, u16 n <= 37, u32 n <= 91, u64 n <= 212,
 *            u128 n <= 597, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   Sequence A185422: a(n) enumerates the triangle T(r,k) of forests of
   k increasing plane unary‑binary trees on r nodes, read row by row.  
   The triangle satisfies the recurrence
        T(n+1,k) = T(n,k-1) + k*T(n,k) + k*(k+1)*T(n,k+1),
   with T(1,1)=1 and T(n,k)=0 if k<1 or k>n.
   We compute a(n) by finding the row r and column k that correspond
   to the index n in the flattened triangle, then evaluating
   T(r,k) by dynamic programming.
*/

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#ifndef __cplusplus
/* C implementation ------------------------------------------------------- */
static seq_t* alloc_seq_arr(idx_t size)
{
    return (seq_t*)calloc((size_t)size, sizeof(seq_t));
}
static void free_seq_arr(seq_t* p)
{
    free(p);
}
#else
/* C++ implementation ----------------------------------------------------- */
#include <new>
static seq_t* alloc_seq_arr(idx_t size)
{
    return new seq_t[size];
}
static void free_seq_arr(seq_t* p)
{
    delete[] p;
}
#endif

/* Compute a(n) ------------------------------------------------------------- */
seq_t a(idx_t n)
{
    /* Find the row r that contains the n-th term when the triangle is
       unfolded row by row: triangular numbers.  The loop below runs
       O(sqrt(n)) which is negligible for the expected input sizes.  */
    idx_t r = 1;
    idx_t cumulative = 1;            /* sum_{i=1}^{r} i  */
    while (cumulative < n) {
        ++r;
        cumulative += r;
    }
    idx_t k = n - (cumulative - r);  /* 1‑based column in row r */

    /* Special case: first row */
    if (r == 1) {
        return (seq_t)1;             /* T(1,1) */
    }

    /* Dynamic programming: compute rows 1..r, keeping only the previous
       row (prev) and the current row (cur).  Each row has at most r
       elements, so the total memory consumption is O(r). */
    seq_t* prev = alloc_seq_arr(r + 1);   /* indices 1..r            */
    seq_t* cur  = alloc_seq_arr(r + 1);   /* indices 1..r            */

    /* Base row n = 1 : T(1,1)=1 */
    for (idx_t i = 0; i <= r; ++i) prev[i] = (seq_t)0;
    prev[1] = (seq_t)1;

    for (idx_t i = 2; i <= r; ++i) {      /* build rows 2..r */
        for (idx_t j = 1; j <= i; ++j) {
            seq_t left  = (j > 1)        ? prev[j-1] : (seq_t)0;
            seq_t mid   = (i-1 >= j)     ? prev[j] * (seq_t)j       : (seq_t)0;
            seq_t right = (j+1 <= i-1)   ? prev[j+1] * (seq_t)j * (seq_t)(j+1) : (seq_t)0;
            cur[j] = left + mid + right;
        }
        /* Prepare for next iteration: swap prev and cur */
        seq_t* tmp = prev;
        prev = cur;
        cur  = tmp;
    }

    seq_t result = prev[k];

    /* Clean up */
    free_seq_arr(prev);
    free_seq_arr(cur);

    return result;
}
