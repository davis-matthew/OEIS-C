/*
 * A121547  Fourth slice along the 1-2-plane in the cube a(m,n,o) = a(m-1,n,o)
 *          + a(m,n-1,o) + a(m,n,o-1) for which the first slice is Pascal's
 *          triangle (slice read by antidiagonals).
 * https://oeis.org/A121547
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 22, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   binom_n_k
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A121547 – fourth slice of a 3‑D Pascal–like cube
 *
 *  a(n) is obtained by indexing the slice on the 1–2‑plane
 *  with antidiagonals.  For a row index r (0‑based) the
 *  row has r+1 elements.  The value at position k (0‑based)
 *  in that row is zero for k = 0, otherwise
 *
 *      a(r,k) = binom(r+2, 3) * binom(r‑1, k‑1)
 *
 *  The linear index n is mapped to its row r by
 *
 *      r*(r+1)/2 ≤ n < (r+1)*(r+2)/2
 *
 *  The binomial coefficient routine binom_n_k is provided
 *  in the utilities of the project.  No other dependencies
 *  are used.
 */

#include "../utils/binom_n_k.h"

seq_t a(idx_t n)
{
    /* Locate the row r that contains index n. */
    idx_t r = 0;
    while ((r + 1) * (r + 2) / 2 <= n)
        ++r;

    /* Position inside the row. */
    idx_t offset = r * (r + 1) / 2;
    idx_t k = n - offset;

    /* Left‑most entry of every row is zero. */
    if (k == 0)
        return 0;

    /* a(r,k) = binom(r+2,3) * binom(r-1,k-1) */
    seq_t part1 = binom_n_k(r + 2, 3);
    seq_t part2 = binom_n_k(r - 1, k - 1);
    return part1 * part2;
}
