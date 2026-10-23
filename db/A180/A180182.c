/*
 * A180182  Triangle read by rows: T(n,k) is the number of compositions of n
 *          without 7's and having k parts; 1 <= k <= n.
 * https://oeis.org/A180182
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = T(r,k) where T(r,k) is the number of compositions of r
 *            without 7's and having k parts. T(r,k) is computed by the
 *            inclusion–exclusion sum: T(r,k) = Σ_{j=max(0,⌈(7k−r)/6⌉)}^k
 *            (−1)^{k−j} · C(k,j) · C(r−7k+7j−1, j−1) (p=7)
 * Range:     u8 n <= 40, u16 n <= 78, u32 n <= 78, u64 n <= 78, u128 n <= 78,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/binom.h"

seq_t a(idx_t n)
{
    /* Map the flat index n to its row r and column k in the triangle.
       The triangle has r rows with entries 1..r. */
    idx_t r = 0;
    idx_t cumulative = 0;          /* number of entries processed so far */
    while (cumulative < n) {
        r++;
        cumulative += r;
    }
    idx_t prev_total = cumulative - r;   /* entries before row r */
    idx_t k = n - prev_total;            /* 1‑based position within the row */

    /* Inclusion–exclusion computation of T(r,k). */
    const idx_t p = 7;
    const idx_t denom = p - 1;           /* 6 */

    /* Lower bound for j */
    idx_t lower;
    if (p * k > r) {
        lower = (p * k - r + denom - 1) / denom;
    } else {
        lower = 0;
    }

    seq_t pos = 0, neg = 0;             /* accumulate positive and negative terms separately */
    for (idx_t j = lower; j <= k; ++j) {
        seq_t binom_k_j = binom(k, j);
        idx_t m = r - p * k + p * j - 1;      /* m = r - 7k + 7j - 1 */
        idx_t rj = j - 1;                     /* j-1 */
        seq_t binom_m_rj = binom(m, rj);
        seq_t term = binom_k_j * binom_m_rj;
        if ((k - j) & 1) {
            neg += term;
        } else {
            pos += term;
        }
    }
    return pos - neg;  /* final result; pos >= neg by construction */
}
