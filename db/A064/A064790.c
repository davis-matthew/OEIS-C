/*
 * A064790  Inverse permutation to A060734.
 * https://oeis.org/A064790
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (i + j - 1)*(i + j - 2)/2 + i where t = floor(sqrt(n -
 *            1)) + 1 i = min(t, t^2 - n + 1) j = min(t, n - (t - 1)^2)
 * Range:     u8 n <= 71, u16 n <= 71, u32 n <= 71, u64 n <= 71, u128 n <= 71,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* integer square root: floor(sqrt(x)) – works for all n<2^64. */
static inline idx_t isqrt_idx(idx_t x)
{
    if (x < 2) return x;          /* sqrt(0)=0, sqrt(1)=1 */
    idx_t low = 1, high = x, ans = 0;
    while (low <= high) {
        idx_t mid = low + (high - low) / 2;
        if (mid <= x / mid) {     /* mid*mid <= x without overflow */
            ans = mid;
            low = mid + 1;
        } else {
            high = mid - 1;
        }
    }
    return ans;
}

seq_t a(idx_t n)
{
    /* t = floor(sqrt(n-1)) + 1, n>=1 so (n-1) never underflows */
    idx_t t = isqrt_idx(n > 0 ? n - 1 : 0) + 1;

    /* i = min(t, t^2 - n + 1) */
    seq_t t_sq = (seq_t)t * (seq_t)t;
    seq_t i_candidate = t_sq - (seq_t)n + 1;
    seq_t i = (i_candidate <= (seq_t)t) ? i_candidate : (seq_t)t;

    /* j = min(t, n - (t - 1)^2) */
    seq_t prev_sq = (seq_t)(t - 1) * (seq_t)(t - 1);
    seq_t j_candidate = (seq_t)n - prev_sq;
    seq_t j = (j_candidate <= (seq_t)t) ? j_candidate : (seq_t)t;

    /* a(n) = (i + j - 1)*(i + j - 2)/2 + i */
    seq_t sum = i + j - 1;
    seq_t term = sum * (sum - 1) / 2 + i;
    return term;
}
