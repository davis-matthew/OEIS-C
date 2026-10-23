/*
 * A186432  Triangle associated with the set S of squares {0,1,4,9,16,...}.
 * https://oeis.org/A186432
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, tabl
 * Formula:   a(n) = T(i,j) where * i = ⌊(√(8n+1)-1)/2⌋ (the row index) * j =
 *            n - i(i+1)/2 (the column index) * T(i,0) = 1, T(i,i) = 1 *
 *            otherwise T(i,j) = 2·binomial(2i, 2j) (OEIS A186432) * * Each
 *            call runs in O(√n) time, which is well below the limits for n ≤
 *            10 000
 * Range:     u8 n <= 16, u16 n <= 48, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>   /* for uint64_t, if needed */

static idx_t isqrt_u64(idx_t n)
{
    /* floor of sqrt(n) using binary search; no 64‑bit overflow */
    idx_t low = 0, high = n;
    while (low <= high) {
        idx_t mid = low + (high - low) / 2;
        if (mid == 0) { low = 1; continue; }
        if (mid <= n / mid)
            low = mid + 1;
        else
            high = mid - 1;
    }
    return high;
}

static seq_t binom_helper(idx_t n, idx_t k)
{
    /* compute binomial(n,k) multiplicatively using exact division */
    if (k > n) return (seq_t)0;
    if (k > n - k) k = n - k;
    seq_t res = 1;
    for (idx_t d = 1; d <= k; ++d) {
        res = res * ((seq_t)(n - k + d)) / (seq_t)d;
    }
    return res;
}

seq_t a(idx_t n)
{
    /* find row index i */
    idx_t i = (isqrt_u64(8 * n + 1) - 1) / 2;

    /* column index j */
    idx_t j = n - i * (i + 1) / 2;

    if (j == 0 || j == i) return (seq_t)1;

    /* interior entry: 2 * binomial(2i, 2j) */
    seq_t val = binom_helper(2 * i, 2 * j);
    return val * (seq_t)2;
}
