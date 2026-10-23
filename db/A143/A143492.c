/*
 * A143492  Unsigned 3-Stirling numbers of the first kind.
 * https://oeis.org/A143492
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  easy, nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 12, u16 n <= 30, u32 n <= 48, u64 n <= 48, u128 n <= 48,
 *            big any n
 * Helpers:   mul_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   A143492: Unsigned 3‑Stirling numbers of the first kind  
   Term a(n) is obtained by listing the triangle row by row:
   a(3)=T(3,3), a(4)=T(4,3), a(5)=T(4,4), a(6)=T(5,3), …
   Recurrence: T(n,k)=T(n-1,k-1)+(n-1)*T(n-1,k) with T(3,3)=1,
   T(3,k)=0 for k>3, and T(n,2)=0 for all n.  
   The function computes a(n) for all n≥3.
*/

#include <stdlib.h>
#include "../utils/mul_small.h"

seq_t a(idx_t n)
{
    /* flat index starts at n=3 */
    if (n < 3) return (seq_t)0;

    /* ---- find row r and column k ------------------------------------------------ */
    /* total elements up to row r is (r-2)(r-1)/2 = m(m+1)/2 where m=r-2   */
    idx_t s = n - 2;                         /* m(m+1)/2 >= s  */
    idx_t low = 0, high = 1;
    /* grow high until it covers s */
    while (high * (high + 1) / 2 < s) high <<= 1;

    while (low < high) {
        idx_t mid = (low + high) / 2;
        if (mid * (mid + 1) / 2 >= s) high = mid;
        else low = mid + 1;
    }
    idx_t m = low;          /* m = r-2   */
    idx_t r = m + 2;        /* target row */
    /* cumulative elements before row r */
    idx_t cumPrev = (r - 3) * (r - 2) / 2;
    idx_t offsetInRow = (n - 3) - cumPrev;      /* 0‑based within the row */
    idx_t k = 3 + offsetInRow;                  /* column index (unused directly) */

    /* ---- build triangle up to row r ------------------------------------------ */
    static seq_t *prev = NULL;     /* holds last built row */
    static idx_t prevRow = 0;      /* row number of prev   */
    static idx_t prevSize = 0;     /* size of prev array   */

    /* initialise for first row if necessary */
    if (prevRow == 0) {
        prevRow = 3;
        prevSize = 1;
        prev = (seq_t *)malloc(prevSize * sizeof(seq_t));
        prev[0] = (seq_t)1;        /* T(3,3) */
    }

    /* if we need a lower row than we have cached, rebuild from scratch */
    if (r < prevRow) {
        free(prev);
        prev = NULL;
        prevRow = 0;
        prevSize = 0;
    }

    /* iteratively build rows until we reach r */
    while (prevRow < r) {
        idx_t newRow   = prevRow + 1;
        idx_t newSize  = newRow - 2;          /* length of this row   */
        seq_t *curr = (seq_t *)malloc(newSize * sizeof(seq_t));
        for (idx_t j = 0; j < newSize; ++j) {
            seq_t val2 = 0;   /* T(prevRow, k-1) */
            if (j > 0 && j - 1 < prevSize) val2 = prev[j - 1];
            seq_t val1 = 0;   /* T(prevRow, k)   */
            if (j < prevSize) val1 = prev[j];
            curr[j] = val2 + mul_small(val1, (unsigned int)(newRow - 1));
        }
        free(prev);
        prev = curr;
        prevSize = newSize;
        prevRow = newRow;
    }

    /* answer is element offsetInRow in row r (0‑based) */
    return prev[offsetInRow];
}
