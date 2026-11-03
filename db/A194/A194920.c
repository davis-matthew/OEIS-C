/*
 * A194920  a(n) = n - floor(n/sqrt(2)).
 * https://oeis.org/A194920
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n - floor(n / sqrt(2))
 * Range:     u8 n <= 15, u16 n <= 401, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 92 terms through n = 7430 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static inline seq_t floor_n_over_sqrt2(idx_t n)
{
    if (n == 0)
        return 0;
    seq_t nseq = (seq_t)n;
    seq_t low   = 0;
    seq_t high  = nseq;
    seq_t ans   = 0;
    while (low <= high) {
        seq_t mid  = low + ((high - low) >> 1);
        seq_t mid2 = mid * mid;
        seq_t left = mid2 << 1;          /* 2*mid^2 */
        seq_t right = nseq * nseq;       /* n^2   */
        if (left <= right) {
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
    if (n == 0)
        return 0;
    return (seq_t)n - floor_n_over_sqrt2(n);
}
