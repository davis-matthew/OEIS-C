/*
 * A090438  Generalized Stirling2 array (4,2).
 * https://oeis.org/A090438
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, tabf
 * Formula:   a(n) is the row‑major entry a(r,k) of the (4,2) generalized
 *            Stirling2 array, where a(r,k) = ((2r)!/k!)*C(2(r-1),k-2) for
 *            k=2..2r. The sequence is read by rows of lengths 1,3,5,…, so the
 *            nth term is stored at (r,k) where r is the unique integer with
 *            (r-1)² < n ≤ r² and k = n-(r-1)²+1
 * Range:     u8 n <= 4, u16 n <= 16, u32 n <= 36, u64 n <= 36, u128 n <= 36,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <math.h>
#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined for n < 1, return 0 */

    /* row r: smallest integer with r² ≥ n */
    idx_t r = (idx_t) sqrt((double)n);
    if (r * r < n) ++r; /* correct rounding errors for some n */

    idx_t prev = (r - 1) * (r - 1);          /* entries before row r */
    idx_t pos   = n - prev;                  /* position within row 1..(2r-1) */
    idx_t kcol  = pos + 1;                   /* actual column index (2..2r) */

    /* compute (2r)!/kcol! = product_{i=kcol+1}^{2r} i  (empty product = 1) */
    seq_t prod = 1;
    for (idx_t i = kcol + 1; i <= 2 * r; ++i)
        prod = prod * (seq_t)i;

    /* compute C(2(r-1), kcol-2) */
    seq_t bin = binomial(2 * (r - 1), kcol - 2);

    return prod * bin;
}
