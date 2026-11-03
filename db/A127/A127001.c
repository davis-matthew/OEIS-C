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
 * Validated: standard, pass, 120 terms through n = 4811 (2026-11-01)
 * Helpers:   ipow_i32
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/ipow_i32.h"

/* Floor square root by binary search */
static inline seq_t floor_sqrt(seq_t x)
{
    seq_t lo = 0, hi = 1;
    while (ipow_i32(hi, 2) <= x)
        hi <<= 1;
    while (lo < hi)
    {
        seq_t mid = (lo + hi) >> 1;
        seq_t mid2 = mid * mid;
        if (mid2 <= x)
            lo = mid + 1;
        else
            hi = mid;
    }
    return lo - 1;
}

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
