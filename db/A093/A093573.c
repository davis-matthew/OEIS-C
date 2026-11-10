/*
 * A093573  Triangle read by rows: row n gives positions where n occurs in the
 *          Golay-Rudin-Shapiro related sequence A020986.
 * https://oeis.org/A093573
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   row r (r>=1) lists, in increasing order, the k with A020986(k) =
 *            r, where A020986(k) = sum_{j=0}^{k} (-1)^popcount(j & (j>>1));
 *            a(n) is entry c of row r with n = r(r-1)/2 + c. Computed by
 *            scanning partial sums
 * Range:     u8 n <= 151, u16 n <= 11325, u32 n <= 11325, u64 n <= 11325,
 *            u128 n <= 11325, big any n
 * Validated: standard, pass, 94 terms through n = 9658 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static inline int A093573_parity(idx_t x)
{
    int p = 0;
    while (x) {
        p ^= 1;
        x &= x - 1;
    }
    return p;
}

seq_t a(idx_t n)
{
    /* find row r: smallest r with r(r+1)/2 >= n */
    idx_t r = 1;
    {
        idx_t lo = 1, hi = 6000000000ULL;
        while (lo < hi) {
            idx_t mid = lo + (hi - lo) / 2;
            if (mid * (mid + 1) / 2 >= n)
                hi = mid;
            else
                lo = mid + 1;
        }
        r = lo;
    }
    idx_t c = n - r * (r - 1) / 2; /* 1-based column */

    long long sum = 0;
    idx_t cnt = 0;
    idx_t k = 0;
    for (;; ++k) {
        if (A093573_parity(k & (k >> 1)) == 0)
            sum += 1;
        else
            sum -= 1;
        if (sum >= 0 && (idx_t)sum == r) {
            ++cnt;
            if (cnt == c)
                return (seq_t)k;
        }
    }
}
