/*
 * A122538  Riordan array (1, x*f(x)) where f(x)is the g.f. of A006318.
 * https://oeis.org/A122538
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 21, u16 n <= 55, u32 n <= 153, u64 n <= 465,
 *            u128 n <= 1540, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A122538
 *  a(n) = the (row‑major) enumeration of the rh triangle
 *         T(n,k) with T(0,0)=1,  T(n,0)=0 (n>0),  T(n,n)=1,
 *         and for 0<k<n:
 *           T(n,k) = T(n-1,k-1) + T(n-1,k) + T(n,k+1).
 *  The sequence a(n) is simply the triangle read row by row
 *  (indexing starts at 0).  For example:
 *  1,
 *  0, 1,
 *  0, 2, 1,
 *  0, 6, 4, 1, …
 *
 *  This implementation computes a(n) by building the required
 *  row of the triangle using only two small arrays; it works
 *  for any seq_t type supported by the build system.
 */

#include <stddef.h>   /* size_t */
#include <stdbool.h>  /* bool */

seq_t a(idx_t n)
{
    /* Find the row r such that r(r+1)/2 <= n < (r+1)(r+2)/2. */
    idx_t r = 0;
    while ((r + 1) * (r + 2) / 2 <= n) {
        ++r;
    }
    idx_t k = n - r * (r + 1) / 2;

    if (r == 0) {
        /* T(0,0) = 1 */
        return (seq_t)1;
    }

    /* A maximum row for n <= 10000 is about 141, so 200 is safe. */
    enum { MAX_R = 200 };
    seq_t prev[MAX_R];
    seq_t curr[MAX_R];

    /* Initialize row 0 */
    size_t lenPrev = 1;   /* length of prev array = current row number + 1 */
    prev[0] = (seq_t)1;

    /* Build rows 1 .. r */
    for (idx_t row = 1; row <= r; ++row) {
        size_t lenCurr = row + 1;
        curr[lenCurr - 1] = (seq_t)1;   /* T(row,row) = 1 */

        /* Compute interior values from right to left */
        for (idx_t col = row - 1; col >= 1; --col) {
            curr[col] = prev[col - 1] + prev[col] + curr[col + 1];
        }

        /* Set the leftmost column */
        curr[0] = (seq_t)0;

        /* Prepare for next iteration: swap prev and curr pointers */
        for (size_t i = 0; i < lenCurr; ++i) {
            prev[i] = curr[i];
        }
        lenPrev = lenCurr;
    }

    /* Result is the k‑th element in the final row */
    return prev[k];
}
