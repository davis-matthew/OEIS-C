/*
 * A054525  Triangle T(n,k): T(n,k) = mu(n/k) if k divides n, T(n,k) = 0
 *          otherwise (n >= 1, 1 <= k <= n).
 * https://oeis.org/A054525
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, tabl
 * Formula:   a(n) = T(r,c) where T(r,c) = μ(r/c) if c|r, else 0. r is chosen
 *            such that r(r+1)/2 ≥ n > (r−1)r/2, i.e. n is the n‑th element
 *            when the lower‑triangular matrix T is read row by row
 * Range:     i8 n <= 1275, i16 n <= 1275, i32 n <= 1275, i64 n <= 1275,
 *            i128 n <= 1275, big any n
 * Helpers:   mobius
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/mobius.h"

seq_t a(idx_t n)
{
    /* Find the row r that contains the element with global index n. */
    idx_t r = 1;
    while ((r * (r + 1)) / 2 < n)
        ++r;

    /* Index of the first element of row r in the flattened sequence. */
    idx_t prev = (r * (r - 1)) / 2;
    /* Column within the triangle (1‑based). */
    idx_t c = n - prev;

    /* Return the Möbius value if c divides r; otherwise 0. */
    if (r % c != 0)
        return (seq_t)0;
    return mobius(r / c);
}
