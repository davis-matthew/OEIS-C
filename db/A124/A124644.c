/*
 * A124644  Triangle read by rows. T(n, k) = binomial(n, k) * CatalanNumber(n
 *          - k).
 * https://oeis.org/A124644
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   a(n) = binomial(r, c) * Catalan(r - c) where * the position
 *            (r,c) in the triangle is obtained from the * 0‑based flattened
 *            index n by * r = floor((sqrt(8*n+1)-1)/2) * c = n - r*(r+1)/2
 * Range:     u8 n <= 20, u16 n <= 54, u32 n <= 172, u64 n <= 563,
 *            u128 n <= 1898, big any n
 * Helpers:   binomial, catalan, floor_isqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/binomial.h"
#include "../utils/catalan.h"
#include "../utils/floor_isqrt.h"

seq_t a(idx_t n)
{
    /* 8*n + 1 fits in idx_t for all supported n */
    idx_t m = (n << 3) + 1;

    /* integer square root of m (floor) */
    idx_t sqrtm = floor_isqrt(m);

    /* row number in the triangle */
    idx_t row = (sqrtm - 1) / 2;

    /* number of items before this row */
    idx_t before = row * (row + 1) / 2;

    /* column number within the row */
    idx_t col = n - before;

    /* compute binomial(row, col) * Catalan(row - col) */
    seq_t bin = binomial(row, col);
    seq_t cat = catalan(row - col);
    return bin * cat;
}
