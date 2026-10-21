/*
 * A078817  Table by antidiagonals giving variants on Catalan sequence:
 *          T(n,k)=C(2n,n)*C(2k,k)*(2k+1)/(n+k+1).
 * https://oeis.org/A078817
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   a(n) = T(r, c) where * T(r, c) = C(2r, r) * C(2c, c) * (2c + 1)
 *            / (r + c + 1) * (r, c) are the 0‑based row and column indices of
 *            the table * obtained by scanning the table of T(r,c) along
 *            antidiagonals. * The mapping from the linear index n to the pair
 *            (r,c) is * provided by antidiagonal_coords(n...
 * Range:     u8 n <= 9, u16 n <= 35, u32 n <= 152, u64 n <= 594,
 *            u128 n <= 2209, big any n
 * Helpers:   antidiagonal_coords, binom_custom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/binom_custom.h"
#include "../utils/antidiagonal_coords.h"

seq_t a(idx_t n)
{
    /* Determine the coordinates (r, c) in the table for the
       given linear index n.  The ordering is by antidiagonals:
       (0,0), (0,1),(1,0), (0,2),(1,1),(2,0) ... */
    idx_t r, c;
    antidiagonal_coords(n, &r, &c);

    /* Compute the central binomials C(2r,r) and C(2c,c). */
    seq_t binom_r = binom_custom(2 * r, r);
    seq_t binom_c = binom_custom(2 * c, c);

    /* Apply the formula:  T = binom_r * binom_c * (2c + 1) / (r + c + 1). */
    seq_t numerator   = binom_r * binom_c;
    numerator        *= (seq_t)(2 * c + 1);
    seq_t denominator = (seq_t)(r + c + 1);

    return numerator / denominator;
}
