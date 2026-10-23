/*
 * A160247  Table read by antidiagonals of "less regular type 1" truncated
 *          octahedron numbers built from face-centered-cubic sphere packing.
 * https://oeis.org/A160247
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, tabl
 * Formula:   v = (2*y^3 + 10*x^3 + 12*y^2*x + 24*y*x^2 - 12*y^2 - 39*x^2 -
 *            48*y*x + 25*y + 47*x - 18)/3 Coordinates (x,y) are obtained by
 *            reading the infinite table by antidiagonals, starting with (1,1)
 *            for n = 1
 * Range:     u8 n <= 10, u16 n <= 36, u32 n <= 36, u64 n <= 36, u128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;           /* off‑by‑one guard – offset is 1 */

    /* n-1 -> 0‑based index into the antidiagonal enumeration */
    idx_t k = n - 1;

    /* Find the diagonal s (sum = x + y) that contains k.
       The first diagonal has s = 2 (x=y=1).  All diagonals after
       the m‑th one have m terms (s = m+1).  The number of terms up
       to diagonal s (exclusive) is T = (s-2)*(s-1)/2. */
    idx_t s = 2;
    while ((s - 1) * s / 2 <= k) ++s;

    /* Terms before this diagonal */
    idx_t prev = (s - 2) * (s - 1) / 2;
    idx_t offset = k - prev;          /* 0 ≤ offset < (s-1) */

    /* Coordinates on this diagonal, enumerated in decreasing x */
    idx_t x = (s - 1) - offset;
    idx_t y = s - x;

    /* Compute the numerator of the formula using idx_t to avoid
       overflow in the case of small seq_t types. */
    idx_t num =
          (idx_t)2 * y * y * y
        + (idx_t)10 * x * x * x
        + (idx_t)12 * y * y * x
        + (idx_t)24 * y * x * x
        - (idx_t)12 * y * y
        - (idx_t)39 * x * x
        - (idx_t)48 * y * x
        + (idx_t)25 * y
        + (idx_t)47 * x
        - (idx_t)18;

    /* The result is guaranteed to be an integer divisible by 3. */
    return (seq_t)(num / 3);
}
