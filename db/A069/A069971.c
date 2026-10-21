/*
 * A069971  Table by antidiagonals of variance of time for a random walk
 *          starting at 0 to reach one of the boundaries at +n or -k for the
 *          first time.
 * https://oeis.org/A069971
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = n * k * (n^2 + k^2 - 2) / 3 where (n, k) are the
 *            coordinates of index n in the antidiagonal enumeration of the
 *            table T(n,k). The coordinates (n, k) satisfy n ≥ 0, k ≥ 0 and
 *            the index order runs first over n + k = 0, 1, 2, ..., then for
 *            each sum over increasing n (equivalently decreasing k)
 * Range:     u8 n <= 29, u16 n <= 68, u32 n <= 68, u64 n <= 68, u128 n <= 68,
 *            big any n
 * Helpers:   antidiagonal_coords
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/antidiagonal_coords.h"

seq_t a(idx_t n)
{
    idx_t n_coord, k_coord;
    antidiagonal_coords(n, &n_coord, &k_coord);

    /* Promote to seq_t before arithmetic to avoid accidental
       overflow of idx_t operands. */
    seq_t ns = (seq_t)n_coord;
    seq_t ks = (seq_t)k_coord;

    /* Compute ns * ks * (ns*ns + ks*ks - 2) / 3. */
    seq_t prod = ns * ks;
    seq_t inner = ns * ns + ks * ks;   /* ns^2 + ks^2 */
    inner -= (seq_t)2;                  /* ns^2 + ks^2 - 2 */
    prod *= inner;
    prod /= (seq_t)3;                   /* division is exact */

    return prod;
}
