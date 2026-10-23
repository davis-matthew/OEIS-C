/*
 * A176918  Triangle read by rows, a signed variant of A077049 * A128407; as
 *          infinite lower triangular matrices.
 * https://oeis.org/A176918
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  tabl, sign
 * Formula:   a(n) is defined on the triangle T with rows * T(r,1) = -1 for r
 *            > 1 * T(r,j) = -μ(j) if 1 < j < r and j divides r * T(r,j) = 0
 *            otherwise * T(1,1) = 1 * where μ is the Möbius function. * The
 *            sequence a(n) is the triangle read by rows, so for * n = 1 ⇒ r =
 *            1, j = 1 → 1 * n = 2 ⇒ r = 2, j = 1 → -1 *...
 * Range:     i8 n <= 74, i16 n <= 74, i32 n <= 74, i64 n <= 74, i128 n <= 74,
 *            big any n
 * Helpers:   mobius
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/mobius.h"   /* provides seq_t mobius(idx_t m); */

seq_t a(idx_t n)
{
    /* n is 1‑based index */
    if (n == 0)
        return 0;          /* undefined, but safe */

    /* Find the row r such that triangular(r-1) < n ≤ triangular(r) */
    idx_t r = 1;
    idx_t t = 1;          /* t = triangular(r) = r*(r+1)/2 */

    while (t < n) {
        ++r;
        t += r;           /* advance to next triangular number */
    }

    /* Column index j in row r */
    idx_t j = n - (t - r);   /* 1‑based */

    /* Special case: first row */
    if (r == 1)
        return (seq_t)1;        /* a(1) = 1 */

    /* First column of any subsequent row is always -1 */
    if (j == 1)
        return (seq_t)(-1);

    /* Last column (j == r) has value 0 */
    if (j == r)
        return (seq_t)0;

    /* If j does not divide r, entry is 0 */
    if (r % j != 0)
        return (seq_t)0;

    /* j divides r and 1 < j < r: entry equals -μ(j) */
    seq_t mu = mobius(j);   /* μ(j) ∈ {-1,0,1} */
    return -mu;
}
