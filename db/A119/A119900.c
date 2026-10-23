/*
 * A119900  Triangle read by rows: T(n,k) is the number of binary words of
 *          length n with k strictly increasing runs, for 0<=k<=n.
 * https://oeis.org/A119900
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = T(row, col) where row is the largest r with r*(r+1)/2 <=
 *            n, * col = n - r*(r+1)/2, and * T(r,k) = binomial(r+1, 2k - r)
 *            if 2k >= r, * 0 otherwise. * This implements the flattening of
 *            the triangle described by OEIS A119900
 * Range:     u8 n <= 41, u16 n <= 182, u32 n <= 586, u64 n <= 1274,
 *            u128 n <= 1274, big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

static inline idx_t find_row(idx_t n) {
    /* binary search for the largest r with r*(r+1)/2 <= n */
    idx_t low = 0, high = (idx_t)1 << 32;   /* high is safely above max possible r */
    while (low < high) {
        idx_t mid = low + ((high - low + 1) >> 1);
        idx_t tri = mid * (mid + 1) / 2;
        if (tri <= n)
            low = mid;
        else
            high = mid - 1;
    }
    return low;
}

seq_t a(idx_t n) {
    idx_t r = find_row(n);                    /* row index */
    idx_t offset = n - r * (r + 1) / 2;        /* column index within the row */
    idx_t k = offset;

    /* If 2*k < r then the binomial argument would be negative → 0 */
    if ((k << 1) < r)                         /* 2*k < r */
        return 0;

    idx_t j = (k << 1) - r;                   /* j = 2*k - r (non‑negative) */
    /* j cannot exceed r, and r+1 is the upper binomial argument */
    return binomial(r + 1, j);
}
