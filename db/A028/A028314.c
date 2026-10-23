/*
 * A028314  Elements in the 5-Pascal triangle A028313 that are not 1.
 * https://oeis.org/A028314
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = T(r,k) where * T(n,k) = binomial(n+2,k+1) + 3 *
 *            binomial(n,k), * r = the unique integer with r(r+1)/2 ≤ n <
 *            (r+1)(r+2)/2, * k = n - r(r+1)/2. This enumerates the 5‑Pascal
 *            triangle rows * row‑by‑row
 * Range:     u8 n <= 23, u16 n <= 141, u32 n <= 573, u64 n <= 1325,
 *            u128 n <= 1325, big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    /* Find the row r that contains the n‑th element. */
    idx_t r   = 0;   /* current row index */
    idx_t cum = 0;   /* number of elements before row r */

    while (cum + (r + 1) <= n) {
        /* the next row r has length r+1, so add it to the cumulative count */
        cum += r + 1;
        ++r;
    }

    /* column index inside the found row */
    idx_t k = n - cum;

    /* compute the triangle entry */
    seq_t part1 = binomial(r + 2, k + 1);
    seq_t part2 = binomial(r, k);

    return part1 + (seq_t)3 * part2;
}
