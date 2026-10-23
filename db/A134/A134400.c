/*
 * A134400  M * A007318, where M = triangle with (1, 1, 2, 3, ...) in the main
 *          diagonal and the rest zeros.
 * https://oeis.org/A134400
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(0) = 1; For n > 0 let r be the unique integer such that
 *            r*(r+1)/2 ≤ n < (r+1)*(r+2)/2, and let k = n - r*(r+1)/2. Then
 *            a(n) = r * binomial(r, k), where binomial is the standard
 *            binomial coefficient. This yields the flattened triangle T(r,k)
 *            = r*binomial(r,k) with T(0,0)=1
 * Range:     u8 n <= 38, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    /* Special‑case the first element */
    if (n == 0)
        return (seq_t)1;

    /* Find row r and column k for the given linear index n.
       The row length is r+1, so we accumulate row sizes
       until the cumulative count exceeds n. */
    idx_t r   = 0;
    idx_t cum = 0;               /* number of entries before current row */
    while (cum + r + 1 <= n) {
        cum += r + 1;
        r   += 1;
    }

    /* Position inside row r */
    idx_t k = n - cum;

    /* T(r,k) = r * binomial(r,k) for r > 0 */
    return (seq_t)r * binomial(r, k);
}
