/*
 * A026998  Triangular array T read by rows: T(n, k) = t(n, 2k), t given by
 *          A027960, 0 <= k <= n, n >= 0.
 * https://oeis.org/A026998
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) is the triangular array T read by rows. For n ≥ 0 let r =
 *            floor((sqrt(8n+1)-1)/2) and k = n - r*(r+1)/2. Then if 2*k ≤ r:
 *            a(n) = Lucas(2*k+1) else: a(n) = Σ_{t=0}^{2(r-k)}
 *            Lucas(2(r-k)-t+1) * C( (2*k-r)+(t)-1 , t )
 * Range:     u8 n <= 50, u16 n <= 164, u32 n <= 617, u64 n <= 1325,
 *            u128 n <= 1325, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

/* Helper: Lucas number for index idx (Lucas(0)=2, Lucas(1)=1).  */
static seq_t lucas_value(idx_t idx)
{
    if (idx == 0) return (seq_t)2;
    if (idx == 1) return (seq_t)1;
    seq_t a = (seq_t)2, b = (seq_t)1, c;
    for (idx_t i = 2; i <= idx; ++i) {
        c = a + b;
        a = b;
        b = c;
    }
    return b;
}

/* Main sequence function.  */
seq_t a(idx_t n)
{
    /* Find row r such that r*(r+1)/2 ≤ n < (r+1)*(r+2)/2.  */
    idx_t r = 0;
    if (n > 0) {
        idx_t lo = 0, hi = n;
        while (lo <= hi) {
            idx_t mid = lo + ((hi - lo) >> 1);
            __uint128_t lhs = (__uint128_t)mid * (mid + 1);
            __uint128_t rhs = (__uint128_t)n * 2;
            if (lhs <= rhs) {
                r = mid;
                lo = mid + 1;
            } else {
                if (mid == 0) break;
                hi = mid - 1;
            }
        }
    }

    /* Column index k.  */
    __uint128_t used = (__uint128_t)r * (r + 1) / 2;
    idx_t k = n - (idx_t)used;

    /* 2*k <= r: use Lucas(2*k+1).  */
    if ((k << 1) <= r) {
        idx_t lucas_idx = (idx_t)((k << 1) + 1);
        return lucas_value(lucas_idx);
    }

    /* 2*k > r: compute the coefficient sum.  */
    idx_t m = (idx_t)(2 * (r - k));      /* 0 ≤ m ≤ r */
    idx_t p = (idx_t)(2 * k - r);        /* 1 ≤ p ≤ r   */

    seq_t sum = (seq_t)0;
    seq_t bin = (seq_t)1;               /* bin = C(p-1,0) = 1 */

    for (idx_t t = 0; t <= m; ++t) {
        idx_t j = m - t;                 /* j = m - t → Lucas(j+1) */
        sum += lucas_value(j + 1) * bin;
        if (t < m) {
            bin = bin * (seq_t)(p + t) / (seq_t)(t + 1);
        }
    }
    return sum;
}
