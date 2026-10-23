/*
 * A174158  Triangle read by rows: T(n,m) = (binomial(n - 1, m -
 *          1)*binomial(n, m - 1)/m)^2.
 * https://oeis.org/A174158
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = T(r,m) where r is the row with r(r-1)/2 < n ≤ r(r+1)/2, m
 *            = n - r(r-1)/2, and T(r,m) = (C(r-1,m-1) * C(r,m-1) / m)^2. This
 *            implements the OEIS sequence A174158
 * Range:     u8 n <= 12, u16 n <= 31, u32 n <= 82, u64 n <= 219,
 *            u128 n <= 720, big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/binom.h"

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0; /* undefined for n=0, return 0 */

    /* Find the row r such that r*(r+1)/2 >= n */
    idx_t r = 1;
    while ((r * (r + 1)) / 2 < n) {
        ++r;
    }

    /* Column index within the row (1‑based) */
    idx_t prev_count = (r * (r - 1)) / 2;
    idx_t m = n - prev_count;

    /* Compute the binomial coefficients */
    seq_t b1 = binom(r - 1, m - 1);
    seq_t b2 = binom(r,     m - 1);

    /* (b1 * b2 / m)^2 */
    seq_t prod = b1 * b2;
    seq_t m_seq = (seq_t)m;
    seq_t val = prod / m_seq;
    return val * val;
}
