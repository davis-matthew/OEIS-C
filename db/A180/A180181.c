/*
 * A180181  Triangle read by rows: T(n,k) is the number of compositions of n
 *          without 6's and having k parts; 1 <= k <= n.
 * https://oeis.org/A180181
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) is the (r,c) entry of the triangle reading by rows. The row
 *            r is the smallest integer such that r(r+1)/2 ≥ n, and the column
 *            c = n - (r-1)r/2. T(r,c) counts compositions of r into c parts
 *            avoiding part 6, computed by inclusion–exclusion: T(r,c) =
 *            Σ_{j=0}^{min(c, r/6)} (-1)^j * C(c,j) * C(r-6...
 * Range:     u8 n <= 40, u16 n <= 78, u32 n <= 78, u64 n <= 78, u128 n <= 78,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/binom.h"


static idx_t find_row(idx_t n)
{
    idx_t row = 1;
    idx_t count = 1;               /* total elements up to current row */
    while (count < n) {
        ++row;
        count += row;
    }
    return row;
}

static seq_t T_rc(idx_t n, idx_t k)
{
    if (k == 0 || k > n) return 0;
    const idx_t p = 6;
    seq_t pos = 0, neg = 0;

    idx_t max_j = n / p;
    if (max_j > k) max_j = k;

    for (idx_t j = 0; j <= max_j; ++j) {
        seq_t bin1 = binom(k, j);

        idx_t remaining_sum = n - p * j;

        seq_t bin2 = 0;
        if (k == j) {
            /* no remaining parts – valid only if the sum is 0. */
            if (remaining_sum == 0) bin2 = 1;
        } else {
            if (remaining_sum >= (k - j)) {
                bin2 = binom(remaining_sum - 1, k - j - 1);
            }
        }

        seq_t term = bin1 * bin2;
        if ((j & 1) == 0)
            pos += term;
        else
            neg += term;
    }

    return pos - neg;
}

seq_t a(idx_t n)
{
    if (n < 1) return 0;

    idx_t r = find_row(n);
    idx_t start = (r - 1) * r / 2;
    idx_t c = n - start;   /* 1‑based column */

    return T_rc(r, c);
}
