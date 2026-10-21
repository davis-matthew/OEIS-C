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
 * Helpers:   diag_index
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/diag_index.h"

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
