/*
 * A199479  Triangle T(n,k), read by rows, given by (1,0,0,0,0,0,0,0,0,0,...)
 *          DELTA (1,1,1,0,0,0,0,0,0,0,...) where DELTA is the operator
 *          defined in A084938.
 * https://oeis.org/A199479
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 30, u16 n <= 54, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A199479 – triangle T(n,k) read by rows
 *
 *  Recurrence (derived from the listed generating function):
 *
 *      T(0,0) = 1
 *      T(n,0) = 1  for n ≥ 1
 *      T(n,k) = T(n-1,k) + 3·T(n-1,k-1) – T(n-2,k-1) – T(n-2,k-2)
 *                for 1 ≤ k ≤ n−1
 *      T(n,n) = T(n-1,n-1) + 3·T(n-1,n-2) – T(n-2,n-2) – T(n-2,n-3)
 *
 *  The required sequence a(n) is obtained by flattening the triangle
 *  row by row (row 0 first, then row 1, etc.).
 *
 *  The function below computes a(n) for any n ≥ 0 using only integer
 *  arithmetic and a small amount of dynamic memory.  It does not rely
 *  on any external libraries beyond the standard C library.
 */

#include <stdlib.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    /* Find the row r and column c such that the linear index n lies in
       row r (0‑based) and column c (0‑based).  The offset of row r
       is r(r+1)/2. */
    idx_t r   = 0;
    idx_t off = 0;
    while (off + r + 1 <= n) {
        off += r + 1;
        ++r;
    }
    idx_t c = n - off;

    /* Small rows have trivial answers. */
    if (r == 0) return (seq_t)1;
    if (r == 1) return (c == 0) ? (seq_t)1 : (seq_t)1; /* both entries are 1 */

    /* Allocate three row buffers of length r+1.  The buffers are
       reused for each successive row. */
    size_t sz   = (size_t)(r + 1);
    seq_t *row0 = (seq_t *)malloc(sz * sizeof(seq_t)); /* T(0,0) */
    seq_t *row1 = (seq_t *)malloc(sz * sizeof(seq_t)); /* T(1,0..1) */
    seq_t *row2 = (seq_t *)malloc(sz * sizeof(seq_t)); /* temporary buffer */

    if (!row0 || !row1 || !row2) {
        /* Allocation failure – return 0 as a fallback. */
        free(row0); free(row1); free(row2);
        return (seq_t)0;
    }

    /* Initialize the first two rows. */
    row0[0]     = (seq_t)1;          /* T(0,0) */
    row1[0]     = (seq_t)1;          /* T(1,0) */
    row1[1]     = (seq_t)1;          /* T(1,1) */

    seq_t *prev2 = row0; /* row n-2   */
    seq_t *prev  = row1; /* row n-1   */
    seq_t *curr  = row2; /* row n     */

    for (size_t i = 2; i <= sz - 1; ++i) {
        /* First element of every row is 1. */
        curr[0] = (seq_t)1;

        /* Interior elements 1 … i-1. */
        for (size_t k = 1; k < i; ++k) {
            seq_t a_val = prev[k];
            seq_t b_val = prev[k - 1];
            seq_t c_val = prev2[k - 1];
            seq_t d_val = (k >= 2) ? prev2[k - 2] : (seq_t)0;
            curr[k] = a_val + (seq_t)3 * b_val - c_val - d_val;
        }

        /* Last element i (k = i). */
        curr[i] = (seq_t)3 * prev[i - 1] - prev2[i - 2];

        /* Rotate the row buffers for the next iteration. */
        seq_t *tmp  = prev2;
        prev2 = prev;
        prev  = curr;
        curr  = tmp;
    }

    /* The desired element is in the final 'prev' buffer. */
    seq_t result = prev[(size_t)c];

    /* Clean up allocations. */
    free(row0);
    free(row1);
    free(row2);

    return result;
}
