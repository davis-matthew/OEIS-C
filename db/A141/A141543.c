/*
 * A141543  Triangle T(n,k) read by rows: T(n,2k)=k, T(n,2k+1) = k-n, for
 *          0<=k<=n.
 * https://oeis.org/A141543
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy, tabl
 * Formula:   a(n) = T(r, j) where r = ⌊(√(8n+1)-1)/2⌋ and j = n - r(r+1)/2.
 *            In the triangle T(r, j), T(r, j) = j/2 if j even T(r, j) = j/2 -
 *            r if j odd
 * Range:     i8 n <= 7, i16 n <= 1325, i32 n <= 1325, i64 n <= 1325,
 *            i128 n <= 1325, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Note: seq_t and idx_t are supplied by the build configuration.
   floor_sqrt is a utility that returns the integer square root of a seq_t value. */

#include "../utils/floor_sqrt.h"

seq_t a(idx_t n) {
    /* compute r = floor((sqrt(8n+1)-1)/2) */
    seq_t eight_n_plus_one = (seq_t)n * 8 + 1;
    seq_t sqrt_val = floor_sqrt(eight_n_plus_one);
    idx_t r = (idx_t)((sqrt_val - 1) >> 1);

    /* cumulative count before row r: r*(r+1)/2 */
    idx_t cumulative = (r * (r + 1)) >> 1;

    idx_t j = n - cumulative;           /* position within the row */

    if (j & 1) {                        /* odd j */
        return (seq_t)(j >> 1) - (seq_t)r;
    } else {                            /* even j */
        return (seq_t)(j >> 1);
    }
}
