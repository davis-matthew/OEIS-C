/*
 * A134199  A002260 + A134082 - I as infinite lower triangular matrices; I =
 *          Identity matrix.
 * https://oeis.org/A134199
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 54, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Triangle rule for A134199:
 *   Row 0:            [1]
 *   Row 1:            [3, 2]
 *   Row r ≥ 2:        [1, 2, 3, …, r‑2, 3·r, r+1]
 *   The sequence a(n) is the n‑th element when the rows are written
 *   consecutively.  This implements the above description. */

#include <stdint.h>
#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    /* Determine the row r using the formula for the triangular index:
     *   T_r = r(r+1)/2 ≤ n < T_{r+1}
     *   r = floor((sqrt(8n+1) – 1)/2) */
    seq_t temp   = (seq_t)8 * n + 1;
    seq_t sqrtv  = floor_sqrt(temp);
    idx_t r      = ((idx_t)sqrtv - 1) / 2;

    /* Start index of row r */
    idx_t start = (r * (r + 1)) / 2;
    /* Column within the row (0‑based) */
    idx_t c = n - start;

    /* Apply the row‑specific rule */
    if (c + 1 < r)          /* 0 ≤ c ≤ r‑2: entries 1, 2, …, r‑2 */
        return (seq_t)(c + 1);

    if (c == r - 1)         /* the second‑to‑last entry: 3·r */
        return (seq_t)3 * (seq_t)r;

    /* c == r : the last entry: r + 1 */
    return (seq_t)r + 1;
}
