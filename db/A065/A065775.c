/*
 * A065775  Array T read by diagonals: T(i,j)=least number of knight's moves
 *          on a chessboard (infinite in all directions) needed to move from
 *          (0,0) to (i,j).
 * https://oeis.org/A065775
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = least knight's moves from (0,0) to (i,j) where (i,j) is
 *            the nth coordinate read by anti‑diagonals
 * Range:     u8 n <= 104, u16 n <= 104, u32 n <= 104, u64 n <= 104,
 *            u128 n <= 104, big any n
 * Validated: partial, pass, 105 terms through n = 104 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>

/* ------------------------------------------------------------------
   Compute the diagonal index `k` such that
        k*(k+1)/2 <= n < (k+1)*(k+2)/2
   Uses a binary search with 128‑bit arithmetic to avoid overflow.
   ------------------------------------------------------------------ */
static idx_t diag_index(idx_t n)
{
    idx_t low   = 0;
    idx_t high  = ((idx_t)1 << 32);   /* upper bound: sqrt(2^64) ≤ 2^32 */
    idx_t ans   = 0;

    while (low <= high) {
        idx_t mid = low + (high - low) / 2;
        __uint128_t val = (__uint128_t)mid * (mid + 1) / 2;
        if (val <= n) {
            ans = mid;
            low = mid + 1;
        } else {
            if (mid == 0) break;
            high = mid - 1;
        }
    }
    return ans;
}

/* ------------------------------------------------------------------
   Main sequence function.
   ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    /* 1. Convert index n to anti‑diagonal coordinates (i,j)   */
    idx_t k = diag_index(n);                 /* diagonal number   */
    idx_t prev = k * (k + 1) / 2;            /* elements before k */
    idx_t offset = n - prev;                 /* position in diagonal */
    idx_t i = k - offset;                    /* i ≥ 0 */
    idx_t j = offset;                        /* j ≥ 0 */

    /* 2. Knight distance formula (infinite board)           */
    /*    Use symmetry: order coordinates so x ≥ y ≥ 0         */
    idx_t x = (i > j) ? i : j;
    idx_t y = (i > j) ? j : i;

    /* Special small cases that break the generic formula   */
    if (x == 1 && y == 0) return (seq_t)3;
    if (x == 2 && y == 2) return (seq_t)4;

    idx_t d1 = (x + 1) / 2;                     /* ceil(x/2)      */
    idx_t d2 = (x + y + 2) / 3;                 /* ceil((x+y)/3)  */
    idx_t d  = (d1 > d2) ? d1 : d2;             /* max of the two */

    /* If parity of d differs from parity of (x+y), bump d. */
    if ( (d & 1) != ( (x + y) & 1 ) )
        d += 1;

    return (seq_t)d;
}
