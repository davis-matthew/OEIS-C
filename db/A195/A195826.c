/*
 * A195826  Triangle read by rows with T(n,k) = n - A000217(k), n>=1, k>=1, if
 *          (n - A000217(k))>=0.
 * https://oeis.org/A195826
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   not stated
 * Range:     u8 n <= 76, u16 n <= 76, u32 n <= 76, u64 n <= 76, u128 n <= 76,
 *            big any n
 * Helpers:   kmax_row
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A195826
   Derived from the triangle T(n,k) = n - A000217(k) =
   n - k(k+1)/2, for k≥1 where the result is non‑negative.
   The sequence is produced by reading the triangle row by row
   (row 1 first, then row 2, …).  The following code finds, for any
   index n≥1, the row r and column k such that
        cum(r-1) < n ≤ cum(r),
   where cum(r) = Σ_{i=1..r} floor((√(8i+1)-1)/2).
   It then returns the value n - k(k+1)/2 for that position.  */

#include <stdint.h>
#include <limits.h>
#include "../utils/kmax_row.h"

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n == 0) {                                  /* undefined for n<1 */
        return (seq_t)0;
    }

    idx_t cum = 0;                                   /* number of terms processed so far */

    for (idx_t r = 1;; ++r) {
        idx_t kmax = kmax_row(r);                    /* terms in this row    */

        if (cum + kmax >= n) {                      /* n lies in this row   */
            idx_t offset = n - cum - 1;             /* zero‑based position  */
            idx_t k = offset + 1;                   /* column number        */

            /* value = r - k(k+1)/2 */
            seq_t val = (seq_t)r;
            unsigned __int128 tri = (unsigned __int128)k * (k + 1) / 2;
            val -= (seq_t)tri;
            return val;
        }

        cum += kmax;                                 /* move to next row     */
    }
}
