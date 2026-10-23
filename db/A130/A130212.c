/*
 * A130212  T(k, n) = sum_(1 <= j <= k) [j | k] j mu(k / j) floor(n / k),
 *          triangle read by rows.
 * https://oeis.org/A130212
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   a(n) = ⌊row/col⌋ * φ(col), where (row, col) is the position of n
 *            in the triangle read by rows. Row r has r entries, and the value
 *            at column c of row r is ⌊r/c⌋ multiplied by the Euler totient of
 *            c
 * Range:     u8 n <= 91, u16 n <= 91, u32 n <= 91, u64 n <= 91, u128 n <= 91,
 *            big any n
 * Helpers:   phi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <math.h>
#include <stdint.h>
#include "../utils/phi.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined for offset 1 */

    /* Find the smallest r such that r(r+1)/2 >= n.  */
    double d = sqrt((double)1 + 8.0 * (double)n);
    idx_t r = (idx_t)((-1.0 + d) / 2.0);
    if (r * (r + 1) / 2 < n) ++r;

    /* Column index within row r (1‑based). */
    idx_t prev = (r - 1) * r / 2;
    idx_t col  = n - prev;          /* 1 <= col <= r */

    /* Compute the term: floor(r/col) * φ(col). */
    seq_t phi_col = phi(col);
    seq_t result  = (seq_t)(r / col) * phi_col;
    return result;
}
