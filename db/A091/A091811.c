/*
 * A091811  Array read by rows: T(n,k) =
 *          binomial(n+k-2,k-1)*binomial(2*n-1,n-k).
 * https://oeis.org/A091811
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl, nice
 * Formula:   not stated
 * Range:     u8 n <= 10, u16 n <= 30, u32 n <= 42, u64 n <= 42, u128 n <= 42,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*   a(n)  = T(r, k)  where  T(n, k) = binomial(n + k - 2, k - 1) *
 *                 binomial(2*n - 1, n - k)
 *
 *   n is 1‑based.  For a given linear index n we find the row r and
 *   column k (both 1‑based) such that
 *   n = 1 + 2 + … + (r - 1) + k   ⇔   r(r‑1)/2 < n ≤ r(r+1)/2
 *   and then compute the triangle entry using the formula above.
 */

#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    if (n == 0)           /* offset is 1 – behaviour for n = 0 is irrelevant */
        return 0;

    /* --- find the row r:  r(r-1)/2 < n ≤ r(r+1)/2 --- */
    idx_t r = 1;
    while ((r * (r + 1)) / 2 < n) {
        ++r;
    }

    /* number of terms before this row */
    idx_t prev = (r - 1) * r / 2;

    /* column within the row (1‑based) */
    idx_t k = n - prev;

    /* compute the triangle entry */
    seq_t part1 = binomial(r + k - 2, k - 1);
    seq_t part2 = binomial(2 * r - 1, r - k);

    return part1 * part2;
}
