/*
 * A060102  Bisection of triangle A060098: even-indexed members of column
 *          sequences of A060098 (not counting leading zeros).
 * https://oeis.org/A060102
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, tabl
 * Formula:   * a(n) = Σ_{j=0..⌊(m+1)/2⌋} binom(r + m - j, 2m) * binom(m + 1,
 *            2j) * where the flat index n is first split into a row r and
 *            column m of the * triangular array: * * r must satisfy:
 *            r*(r+1)/2 ≤ n < (r+1)*(r+2)/2 * m = n - r*(r+1)/2 * * This is
 *            the triangle defined by the OEIS A060098, whose flatte...
 * Range:     u8 n <= 22, u16 n <= 53, u32 n <= 53, u64 n <= 53, u128 n <= 53,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <math.h>
#include "../utils/binom.h"

seq_t a(idx_t n) {
    /* Find the row r such that r(r+1)/2 <= n < (r+1)(r+2)/2.
       Use a double approximation and then adjust to get an exact result. */
    double x = sqrt((double)8 * n + 1.0);
    idx_t r = (idx_t)((x - 1.0) / 2.0);
    /* adjust downwards if necessary */
    while (r * (r + 1) / 2 > n) r--;
    /* adjust upwards if we undershot */
    while ((r + 1) * (r + 2) / 2 <= n) r++;
    idx_t tri = r * (r + 1) / 2;
    idx_t m = n - tri;          /* column index within the row */

    idx_t jmax = (m + 1) / 2;
    seq_t sum = 0;

    for (idx_t j = 0; j <= jmax; ++j) {
        seq_t part1 = binom(r + m - j, 2 * m);
        seq_t part2 = binom(m + 1, 2 * j);
        sum += part1 * part2;
    }
    return sum;
}
