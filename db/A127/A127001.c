/*
 * A127001  Zero-one fractional-part array for sqrt(3); a rectangular array T
 *          by antidiagonals.
 * https://oeis.org/A127001
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = ⌊(i+j)·√3⌋ – ⌊i·√3⌋ – ⌊j·√3⌋ where (i, j) are the
 *            antidiagonal coordinates of index n: i = (n – (d–1)d/2) and j =
 *            d – i + 1, with d the smallest integer such that d(d+1)/2 ≥ n.
 *            The floor of k·√3 is computed exactly as ⌊√(3k²)⌋ using the
 *            integer square‑root routine floor_sqrt
 * Range:     u8 n <= 6, u16 n <= 2627, u32 n <= 5050, u64 n <= 5050,
 *            u128 n <= 5050, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    /* Find the diagonal d containing position n. */
    idx_t d = 1;
    idx_t cum = 1;          /* cumulative total up to diagonal d */
    while (cum < n) {
        d++;
        cum += d;           /* cum = d(d+1)/2 after this addition */
    }

    /* Compute previous diagonal cumulative sum to get offset. */
    idx_t prev_cum = cum - d;          /* prev_cum = (d-1)d/2 */
    idx_t offset   = n - prev_cum - 1; /* zero‑based offset within diagonal */

    /* Antidiagonal coordinates (i, j). */
    idx_t i = offset + 1;        /* 1‑based */
    idx_t j = d - offset;        /* 1‑based */

    /* Compute the three floor values using integer sqrt. */
    seq_t n_i   = (seq_t)i;
    seq_t n_j   = (seq_t)j;
    seq_t n_sum = (seq_t)(i + j);

    seq_t fi = floor_sqrt((seq_t)3 * n_i * n_i);
    seq_t fj = floor_sqrt((seq_t)3 * n_j * n_j);
    seq_t fs = floor_sqrt((seq_t)3 * n_sum * n_sum);

    /* The term is the difference of these floors. */
    return fs - fi - fj;
}
