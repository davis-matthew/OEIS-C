/*
 * A133388  Largest integer m such that n-m^2 is a square, or 0 if no such m
 *          exists.
 * https://oeis.org/A133388
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = max{ m | ∃ k ∈ ℕ, m² + k² = n }, 0 if no such m exists
 * Range:     u8 n <= 9602, u16 n <= 9602, u32 n <= 9602, u64 n <= 9602,
 *            u128 n <= 9602, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Integer square root (floor) for idx_t, using binary search. */
static inline idx_t isqrt_idx_t(idx_t n)
{
    idx_t low = 0;
    idx_t high = (n >> 1) + 1;  /* sqrt(n) ≤ n/2 + 1 for n ≥ 1 */
    idx_t mid, ans = 0;
    while (low <= high) {
        mid = (low + high) >> 1;
        /* mid*mid can never overflow since mid ≤ sqrt(n) ≤ (2^32-1). */
        if (mid * mid <= n) {
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
    if (n == 0) return (seq_t)0;  /* defensive, offset 1 but harmless */
    idx_t m = isqrt_idx_t(n);     /* largest possible m */
    while (m > 0) {
        idx_t rem = n - m * m;    /* n - m² */
        idx_t k = isqrt_idx_t(rem);
        if (k * k == rem) {
            return (seq_t)m;      /* found representation */
        }
        --m;
    }
    return (seq_t)0;              /* no representation */
}
