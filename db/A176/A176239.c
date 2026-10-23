/*
 * A176239  Shifted signed Catalan triangle T(n,k) =
 *          (-1)^*(n+k+1)*A009766(n,k-n+1) read by rows.
 * https://oeis.org/A176239
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  tabf, sign
 * Formula:   a(n) is the row‑major flattening of the shifted signed Catalan
 *            triangle T(n,k) defined by T(n,k) = 0 if k <= n-2 T(n,k) =
 *            (-1)^(n+k+1) * (binom(n+m,m) - binom(n+m,m-1)) for k = n-1 …
 *            2n-1, where m = k-n+1 (0 ≤ m ≤ n) T(n,2n) = 0 T(n,2n+1) =
 *            (-1)^(n+1) * Catalan(n+1) (# of rows = 2(n+1)) Offsets a...
 * Range:     i8 n <= 26, i16 n <= 71, i32 n <= 71, i64 n <= 71, i128 n <= 71,
 *            big any n
 * Helpers:   binom, catalan
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/binom.h"
#include "../utils/catalan.h"

seq_t a(idx_t n) {
    /* Determine row r such that r*(r+1) <= n < (r+1)*(r+2) */
    idx_t r = 0;
    while ((r + 1) * (r + 2) <= n) ++r;

    idx_t start = r * (r + 1);
    idx_t k = n - start;                 /* 0 <= k < 2*(r+1) */

    /* values with k <= r-2 are zero (handle underflow of unsigned) */
    if (r >= 2 && k <= r - 2) {
        return 0;
    }

    /* middle band: n-1 <= k <= 2n-1 */
    if (k >= r - 1 && k <= 2 * r - 1) {
        idx_t m = k - r + 1;             /* 0 <= m <= r */
        seq_t bin1 = binom(r + m, m);
        seq_t bin2 = 0;
        if (m > 0) {
            bin2 = binom(r + m, m - 1);
        }
        seq_t val = bin1 - bin2;         /* A009766(n,m) */
        /* apply sign (-1)^(r+k+1) */
        if (((r + k + 1) & 1) != 0) {
            val = -val;
        }
        return val;
    }

    /* k == 2n */
    if (k == 2 * r) {
        return 0;
    }

    /* k == 2n+1 (boundary case) */
    seq_t catal = catalan(r + 1);
    if (((r + 1) & 1) != 0) {
        catal = -catal;
    }
    return catal;
}
