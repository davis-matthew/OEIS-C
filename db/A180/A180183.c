/*
 * A180183  Triangle read by rows: T(n,k) is the number of compositions of n
 *          without 8's and having k parts; 1 <= k <= n.
 * https://oeis.org/A180183
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   T(n,k) = sum_{j=max(0,ceil((p*k-n)/(p-1)))}^{k} (-1)^(k-j) * *
 *            C(k,j) * C(n-p*k + p*j - 1, j-1), with p = 8. * a(n) is the
 *            flattened triangle read row by row
 * Range:     u8 n <= 40, u16 n <= 78, u32 n <= 78, u64 n <= 78, u128 n <= 78,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/binomial.h"

static inline idx_t _row_start(idx_t r)
{
    /* first linear index of row r (1‑based) */
    return ((r * (r - 1)) / 2) + 1;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* defined only for n≥1 in OEIS */

    /* locate the row r and column k for linear index n */
    idx_t lo = 1, hi = n;
    idx_t row = 1;
    while (lo <= hi) {
        idx_t mid = lo + ((hi - lo) >> 1);
        idx_t start_mid = _row_start(mid);
        if (start_mid <= n) {
            row = mid;
            lo = mid + 1;
        } else {
            hi = mid - 1;
        }
    }

    idx_t start = _row_start(row);
    idx_t col   = n - start + 1;     /* 1‑based column in the row */

    /* parameters for the formula */
    const idx_t p = 8;              /* forbidden part */
    const idx_t sum = row;          /* n in the combinatorial interpretation */
    const idx_t k   = col;           /* number of parts */

    /* lower bound for j */
    int64_t diff = (int64_t)p * (int64_t)k - (int64_t)sum;
    idx_t jmin;
    if (diff <= 0)
        jmin = 0;
    else
        jmin = (idx_t)((diff + (p - 2)) / (p - 1));   /* ceil division */

    if (jmin > k) return 0;

    seq_t pos_sum = 0;
    seq_t neg_sum = 0;

    for (idx_t j = jmin; j <= k; ++j) {
        seq_t prod;
        if (j == 0) {
            /* handle binomial(top, -1) which is 1 iff n == p*k */
            if (sum == p * k) {
                prod = 1;
            } else {
                continue;            /* term contributes 0 */
            }
        } else {
            int64_t top_val = (int64_t)sum - (int64_t)p * (int64_t)k
                            + (int64_t)p * (int64_t)j - 1;
            if (top_val < (int64_t)(j - 1))
                continue;
            idx_t top = (idx_t)top_val;
            seq_t c1 = binomial(k, j);
            seq_t c2 = binomial(top, j - 1);
            prod = c1 * c2;
        }

        if (((k - j) & 1) == 0)
            pos_sum += prod;
        else
            neg_sum += prod;
    }

    return pos_sum - neg_sum;
}
