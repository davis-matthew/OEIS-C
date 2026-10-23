/*
 * A132775  A007818 * A132774.
 * https://oeis.org/A132775
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 23, u16 n <= 54, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A132775:
 * Triangle T(n,k) = (2n+1) * C(n,k) flattened row‑wise.
 * Thus, for an index n (0‑based):
 *   let r = ⌊(√(8n+1)−1)/2⌋   (row number)
 *   let k = n − r(r+1)/2       (column within that row)
 *   a(n) = (2r+1) * binomial(r, k)
 */
#include <stdint.h>
#include "../utils/binomial.h"

seq_t a(idx_t n) {
    /* Find the row r such that r(r+1)/2 ≤ n < (r+1)(r+2)/2 */
    idx_t r = 0;
    while (((unsigned __int128)(r + 1) * (r + 2) / 2) <= n) {
        ++r;
    }

    /* Starting index of row r */
    idx_t start = (unsigned __int128)r * (r + 1) / 2;

    /* Column k within the row */
    idx_t k = n - start;

    /* Binomial coefficient C(r,k) */
    seq_t binom = binomial(r, k);

    /* Multiply by (2r+1) */
    return binom * (seq_t)(2 * r + 1);
}
