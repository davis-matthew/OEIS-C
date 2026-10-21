/*
 * A211430  Number of ordered triples (w,x,y) with all terms in
 *          {-n,...,0,...,n} and w^2+x+y=0.
 * https://oeis.org/A211430
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{w=-n}^{n} max(0, 2n - w² + 1)
 * Range:     u8 n <= 16, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* 2n and 2n+1 in seq_t */
    seq_t twon  = (seq_t)2 * n;
    seq_t base  = twon + 1;  /* value for w = 0 */
    seq_t result = base;

    /* binary search for the largest m such that m² ≤ 2n+1 */
    seq_t limit_seq = twon + 1;      /* 2n+1 as seq_t */
    idx_t low  = 0;
    idx_t high = n;                  /* sqrt(2n+1) ≤ n for n≥1 */
    idx_t m = 0;

    while (low <= high) {
        idx_t mid = (low + high) >> 1;
        seq_t mid2 = (seq_t)mid * (seq_t)mid;
        if (mid2 <= limit_seq) {
            m = mid;
            low = mid + 1;
        } else {
            if (mid == 0) break;
            high = mid - 1;
        }
    }

    /* Sum contributions for w = ±1 … ±m */
    for (idx_t w = 1; w <= m; ++w) {
        seq_t add = twon - (seq_t)w * w + 1;  /* 2n - w² + 1  */
        result += add * 2;                    /* double for ±w */
    }

    return result;
}
