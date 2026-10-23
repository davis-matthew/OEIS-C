/*
 * A174376  Triangle T(n, k) = n!*q^k/(n-k)! if floor(n/2) > k-1 otherwise
 *          n!*q^(n-k)/k!, with q = 2, read by rows.
 * https://oeis.org/A174376
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   a(n) = T(r,k) where r = floor((sqrt(8*n+1)-1)/2), k = n -
 *            r*(r+1)/2, T(r,k) = product_{i=0}^{min(k,r-k)-1} (r-i) *
 *            2^{min(k,r-k)}
 * Range:     u8 n <= 23, u16 n <= 58, u32 n <= 142, u64 n <= 362,
 *            u128 n <= 1011, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* integer square root for unsigned 64-bit values
   Returns floor(sqrt(v)) */
static idx_t isqrt_idx_t(idx_t v)
{
    idx_t lo = 0, hi = v, ans = 0;
    while (lo <= hi) {
        idx_t mid = (lo + hi) >> 1;
        unsigned __int128 sq = (unsigned __int128)mid * mid;
        if (sq <= v) {
            ans = mid;
            lo  = mid + 1;
        } else {
            if (mid == 0) break;
            hi = mid - 1;
        }
    }
    return ans;
}

seq_t a(idx_t n)
{
    /* step 1: find row r */
    idx_t temp = (n << 3) + 1;                /* 8*n + 1 */
    idx_t sqrt_val = isqrt_idx_t(temp);      /* floor(sqrt(8*n+1)) */
    idx_t r = (sqrt_val - 1) >> 1;            /* floor((sqrt-1)/2) */

    /* step 2: find position k within row r */
    idx_t prev_terms = (r * (r + 1)) >> 1;    /* r*(r+1)/2 */
    idx_t k = n - prev_terms;                /* 0 <= k <= r */

    /* step 3: compute m = min(k, r-k) */
    idx_t m = (k <= r - k) ? k : (r - k);

    /* step 4: compute result = product_{i=0}^{m-1} (r-i) * 2^m */
    seq_t result = 1;
    for (idx_t i = 0; i < m; ++i) {
        result *= (seq_t)(r - i);
        result *= 2;                         /* multiply by 2 */
    }
    return result;
}
