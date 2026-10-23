/*
 * A026836  Triangular array T read by rows: T(n,k) = number of partitions of
 *          n into distinct parts, the greatest being k, for k=1,2,...,n.
 * https://oeis.org/A026836
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = number of partitions of the row number r into distinct
 *            parts with largest part k, where r = minimal integer with
 *            r(r+1)/2 ≥ n and k = n - r(r-1)/2. Computed by dynamic subset
 *            sum
 * Range:     u8 n <= 90, u16 n <= 90, u32 n <= 90, u64 n <= 90, u128 n <= 90,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <math.h>

static idx_t row_of_index(idx_t idx)
{
    if (idx == 0) return 0;
    idx_t r = (idx_t)((sqrt(8.0L * idx + 1.0L) - 1.0L) / 2.0L);
    while (r * (r + 1) / 2 < idx) ++r;
    while (r > 0 && (r - 1) * r / 2 >= idx) --r;
    return r;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;

    idx_t r = row_of_index(n);
    idx_t prev = (r - 1) * r / 2;
    idx_t k = n - prev;

    /* Case k = 1 (only the partition [1] when r == 1) */
    if (k == 1) return (r == 1) ? 1 : 0;

    idx_t target = r - k;
    if (target == 0) return 1;

    /* DP for subset sums of {1,…,k‑1} that sum to target */
    static const size_t MAX_TARGET = 10000;       /* sufficient for n up to ~10⁴ */
    seq_t dp[MAX_TARGET + 1];

    size_t t = (size_t)target;
    for (size_t i = 0; i <= t; ++i)
        dp[i] = 0;
    dp[0] = 1;

    for (idx_t i = 1; i < k; ++i) {
        size_t ii = (size_t)i;
        for (size_t s = t; s >= ii; --s)
            dp[s] += dp[s - ii];
    }
    return dp[t];
}
