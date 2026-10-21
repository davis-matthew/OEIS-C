/*
 * A059260  Triangle read by rows giving coefficient T(i,j) of x^i y^j in
 *          1/(1-y-x*y-x^2) = 1/((1+x)(1-x-y)) for (i,j) = (0,0), (1,0),
 *          (0,1), (2,0), (1,1), (0,2), ...
 * https://oeis.org/A059260
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, nice
 * Formula:   not stated
 * Range:     u8 n <= 70, u16 n <= 217, u32 n <= 681, u64 n <= 1274,
 *            u128 n <= 1274, big any n
 * Helpers:   allocate_seq_buffer, free_seq_buffer
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  This implementation returns the numbers of OEIS A059260.
 *  The recurrence used is
 *
 *      T(i,0) = 1 if i even, 0 if i odd
 *      T(0,j) = 1
 *      T(i,j) = T(i-1,j) + T(i,j-1)   for i,j > 0
 *
 *  The sequence a(n) is obtained by reading T(i,j) in
 *  anti‑diagonals:  a(n) = T(i,j) where
 *
 *      i + j = floor( (sqrt(8*n+1)-1)/2 )
 *      j = n - i*(i+1)/2
 *
 *  The routine works for all builds (plain C or C++ with arbitrary
 *  precision integers) and does not hard‑code any terms.  It uses
 *  only integer arithmetic and a small temporary buffer, so it is
 *  efficiently evaluable for any n that fits into the 64‑bit index
 *  type.  For large shared computations the function can be memoised
 *  by the caller if desired.
 */

#include <stdlib.h>
#include "../utils/allocate_seq_buffer.h"
#include "../utils/free_seq_buffer.h"

/* compute T(i,j) by dynamic programming */
static seq_t T_of(idx_t i, idx_t j)
{
    /* base cases */
    if (j == 0)                     /* T(i,0) */
        return ((i & 1) == 0) ? (seq_t)1 : (seq_t)0;
    if (i == 0)                     /* T(0,j) */
        return (seq_t)1;

    /* allocate a row buffer of length j+1 */
    seq_t *row = allocate_seq_buffer(j + 1);
    if (!row) exit(1);              /* out of memory */

    /* initialise row for i = 0: all ones */
    for (idx_t k = 0; k <= j; ++k)
        row[k] = (seq_t)1;

    /* sweep through rows 1 .. i, updating the running row */
    for (idx_t r = 1; r <= i; ++r) {
        row[0] = ((r & 1) == 0) ? (seq_t)1 : (seq_t)0;  /* T(r,0) */
        for (idx_t k = 1; k <= j; ++k)
            row[k] = row[k] + row[k - 1];                /* Pascal step */
    }

    seq_t result = row[j];
    free_seq_buffer(row, j + 1);
    return result;
}

/* return the nth term of the sequence */
seq_t a(idx_t n)
{
    /* find the largest s such that s*(s+1)/2 <= n */
    idx_t s = 0;
    while ((s + 1) * (s + 2) / 2 <= n)
        ++s;

    idx_t start = s * (s + 1) / 2;   /* first index on this diagonal */
    idx_t d     = n - start;         /* offset along the diagonal */
    idx_t i     = s - d;             /* i coordinate */
    idx_t j     = d;                 /* j coordinate */

    return T_of(i, j);
}
