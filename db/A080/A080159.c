/*
 * A080159  Triangular array of ways of drawing k non-intersecting chords
 *          between n points on a circle; i.e., Motzkin polynomial
 *          coefficients.
 * https://oeis.org/A080159
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = T(r,k) where r(r+1)/2 <= n < (r+1)(r+2)/2, * k = n -
 *            r(r+1)/2. * T(r,k) = 0 if r < 2k * T(r,k) = binom(r,2k) *
 *            binom(2k,k)/(k+1) otherwise
 * Range:     u8 n <= 47, u16 n <= 90, u32 n <= 90, u64 n <= 90, u128 n <= 90,
 *            big any n
 * Helpers:   binom_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/binom_mul.h"

seq_t a(idx_t n)
{
    /* Locate the row r such that r(r+1)/2 <= n < (r+1)(r+2)/2. */
    idx_t r = 0;
    while (((r + 1) * (r + 2)) / 2 <= n)
        ++r;

    /* Index of the first entry in this row. */
    idx_t start = r * (r + 1) / 2;

    /* Column index k within the row. */
    idx_t k = n - start;

    /* Entries where r < 2k are zero. */
    if (r < 2 * k) return 0;

    /* Compute T(r,k) = binom(r,2k) * binom(2k,k)/(k+1). */
    seq_t bin = binom_mul(r, 2 * k);
    seq_t cat = binom_mul(2 * k, k);
    cat = cat / ((seq_t)(k + 1));

    return bin * cat;
}
