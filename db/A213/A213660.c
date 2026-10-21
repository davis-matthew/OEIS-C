/*
 * A213660  Irregular triangle read by rows: T(n,k) is the number of
 *          dominating subsets with k vertices of the graph G(n) obtained by
 *          taking n copies of the cycle graph C_3 with a vertex in common.
 * https://oeis.org/A213660
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   a(n) is the n-th entry of the irregular triangle T(m,k) given by
 *            T(m,k) = 2^(2m‑k) * binomial(m, k‑m) + binomial(2m, k‑1) for 1 ≤
 *            k ≤ 2m+1; the triangle is read row‑by‑row starting at n=1. The
 *            function computes the row index m and the column index k from n
 *            and then applies the above closed‑form
 * Range:     u8 n <= 19, u16 n <= 63, u32 n <= 63, u64 n <= 63, u128 n <= 63,
 *            big any n
 * Helpers:   binomial, floor_sqrt, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include "../utils/floor_sqrt.h"
#include "../utils/binomial.h"
#include "../utils/pow2.h"

/* Find the row m such that the (m‑1)‑st row ends before position n
   and the m‑th row contains position n.  The cumulative count up to
   row r equals r(r+2).  Thus we look for the smallest r with r(r+2) >= n. */
static idx_t find_row(idx_t n)
{
    idx_t low = 1;
    /* The row index never exceeds sqrt(2n) + 1, so use a small upper bound. */
    idx_t high = (idx_t)floor_sqrt((seq_t)n) + 3;
    while (low < high) {
        idx_t mid = low + ((high - low) >> 1);
        seq_t val = (seq_t)mid * ((seq_t)mid + 2);   /* mid*(mid+2) */
        if (val >= (seq_t)n)
            high = mid;
        else
            low = mid + 1;
    }
    return low;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;              /* defined only for n >= 1 */

    /* Determine row index m and column index k. */
    idx_t m = find_row(n);
    idx_t prev_total = (m - 1) * (m + 1);   /* elements in rows 1..m-1 */
    idx_t k = n - prev_total;              /* 1‑based position within row m */

    /* T(m,k) = 2^(2m‑k) * binomial(m, k‑m) + binomial(2m, k‑1) */
    seq_t first = 0;
    if (k <= 2 * m) {               /* first summand only for k ≤ 2m */
        idx_t exp = 2 * m - k;
        seq_t bin = 0;
        if (k >= m)                 /* binomial(m, k‑m) is non‑zero when k ≥ m */
            bin = binomial(m, k - m);
        if (bin)                     /* avoid 0 * pow2 */
            first = pow2(exp) * bin;
    }

    seq_t second = binomial(2 * m, k - 1);

    return first + second;
}
