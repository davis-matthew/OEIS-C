/*
 * A104855  Triangle read by rows: T(n,k) (0 <= k <= n) is the number of
 *          three-dimensional lattice walks consisting of n unit steps, each
 *          in one of the six coordinate directions, starting at the origin,
 *          never going below the horizontal plane and having k vertical
 *          steps.
 * https://oeis.org/A104855
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = binomial(r,c) * binomial(c, floor(c/2)) * 4^(r-c), where
 *            r = row_of(n) and c = n - r*(r+1)/2 (the column index)
 * Range:     u8 n <= 9, u16 n <= 35, u32 n <= 46, u64 n <= 46, u128 n <= 46,
 *            big any n
 * Helpers:   binom, pow2, row_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/binom.h"
#include "../utils/pow2.h"
#include "../utils/row_of.h"

seq_t a(idx_t n) {
    /* find the row containing index n */
    idx_t r = row_of(n);

    /* column index: c = n - (r*(r+1)/2) */
    idx_t triangular = (idx_t)((unsigned __int128)r * (r + 1) / 2);
    idx_t c = n - triangular;

    /* compute T(r,c) = binom(r,c) * binom(c, floor(c/2)) * 4^(r-c) */
    seq_t part1 = binom(r, c);
    seq_t part2 = binom(c, c / 2);
    seq_t part3 = pow2((uint64_t)(2 * (r - c))); /* 4^(r-c) = 2^(2*(r-c)) */

    return part1 * part2 * part3;
}
