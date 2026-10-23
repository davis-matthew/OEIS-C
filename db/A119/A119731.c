/*
 * A119731  f-Pascal's triangle where f(n) = n^3 = A000578(n).
 * https://oeis.org/A119731
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 10, u16 n <= 15, u32 n <= 21, u64 n <= 21, u128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   a(n) = the n-th element (row‑major) of the f‑Pascal triangle
   where f(i) = i³, i.e. T(0,0)=1 and
   T(n,k) = T(n-1,k-1)³ + T(n-1,k)³ for 0≤k≤n.
   This function recomputes the required row and returns the
   requested element.
*/

#include <stdlib.h>
#include <stdint.h>

/* Return the row index r such that the total number of
   elements up to and including row r (starting from row 0)
   exceeds n.  The total in rows 0..r is (r+1)(r+2)/2. */
static inline idx_t row_of(idx_t n)
{
    idx_t r = 0;
    while ((r + 1) * (r + 2) / 2 <= n)
        ++r;
    return r;
}

seq_t a(idx_t n)
{
    /* Find which row and column contain the n-th element. */
    idx_t r = row_of(n);                       /* row index   */
    idx_t prev_total = r * (r + 1) / 2;        /* entries before row r */
    idx_t k = n - prev_total;                  /* column index */

    /* Allocate storage for the current and next row. */
    size_t len = r + 1;
    seq_t *cur  = (seq_t *)malloc(len * sizeof(seq_t));
    seq_t *next = (seq_t *)malloc(len * sizeof(seq_t));
    if (!cur || !next)
    {
        free(cur);
        free(next);
        return (seq_t)0;
    }

    /* Initialize row 0. */
    cur[0] = (seq_t)1;

    /* Build rows 1 .. r. */
    for (idx_t row = 1; row <= r; ++row)
    {
        for (idx_t j = 0; j <= row; ++j)
        {
            seq_t left  = (j > 0)   ? cur[j-1] : (seq_t)0;
            seq_t right = (j < row) ? cur[j]   : (seq_t)0;
            seq_t cube_left  = left  * left  * left;
            seq_t cube_right = right * right * right;
            next[j] = cube_left + cube_right;
        }
        /* Prepare for next iteration. */
        seq_t *tmp = cur;
        cur = next;
        next = tmp;
    }

    /* The answer is the k‑th entry of the last computed row. */
    seq_t result = cur[k];

    /* Clean up. */
    free(cur);
    free(next);
    return result;
}
