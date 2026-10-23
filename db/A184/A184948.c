/*
 * A184948  Triangle read by rows: SM(n,m) is the number of symmetric 0-1
 *          matrices of order n such that the total number of 1's is m (n >=
 *          1, 0 <= m <= n^2).
 * https://oeis.org/A184948
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   a(n) = SM(r,m) where r and m are obtained by flattening the
 *            triangle of symmetric 0‑1 matrices. SM(r,m) = Σ_{k=0..r, k≡m
 *            (mod 2)} binom(r,k) * binom(r*(r-1)/2,(m-k)/2)
 * Range:     u8 n <= 39, u16 n <= 72, u32 n <= 177, u64 n <= 559,
 *            u128 n <= 1360, big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binom.h"

seq_t a(idx_t n) {
    /* undefined for indices below the offset (n < 1) */
    if (n < 1) return 0;

    /* Find the row r that contains the n‑th entry.  Row r has
       r^2 + 1 entries (m ranges from 0 to r^2). */
    idx_t r   = 1;
    idx_t pos = 0;          /* cumulative count of entries before current row */
    while (1) {
        idx_t len = r * r + 1;
        if (pos + len >= n) break;
        pos += len;
        ++r;
    }

    /* Position within row (1‑based).  Convert to 0‑based m = position-1. */
    idx_t m = n - pos - 1;

    /* Number of off‑diagonal unordered pairs. */
    idx_t R = r * (r - 1) / 2;

    seq_t result = 0;
    for (idx_t k = 0; k <= r; ++k) {
        if (m < k) continue;            /* cannot have more diagonal ones than total */
        if (((m ^ k) & 1) != 0) continue;/* parity check: m-k must be even */
        idx_t t = (m - k) >> 1;          /* number of ones in off‑diagonal pairs */
        if (t > R) continue;
        seq_t c1 = binom(r, k);          /* choose diagonal ones */
        seq_t c2 = binom(R, t);          /* choose off‑diagonal ones */
        result += c1 * c2;
    }

    return result;
}
