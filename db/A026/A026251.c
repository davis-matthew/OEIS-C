/*
 * A026251  a(n) = |s(n) - n|, where s = A026250. Also a(n) = 2*t(n), where t
 *          = A026249.
 * https://oeis.org/A026251
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2 * k, where k is the unique integer satisfying n =
 *            ⌊k·√2⌋ or n = ⌊k·√2⌋ + 2k (k ≥ 1). The two positions correspond
 *            to the first and second occurrence of k in the sequence
 * Range:     u8 n <= 63, u16 n <= 63, u32 n <= 63, u64 n <= 63, u128 n <= 63,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <math.h>

/* Integer square root for a 128‑bit unsigned integer.
   Returns ⌊√x⌋.  This routine uses long double for a close
   initial estimate and then corrects it. */
static inline idx_t isqrt_u128(__int128 x) {
    long double d = sqrt((long double)x);
    idx_t r = (idx_t)d;
    while ((__int128)(r + 1) * (r + 1) <= x) ++r;
    while ((__int128)r * r > x) --r;
    return r;
}

/* Find k such that ⌊k·√2⌋ == n, or return 0 if none exists.  */
static idx_t find_k_by_first(idx_t n) {
    idx_t lo = 1, hi = n;          /* lo <= hi   */
    while (lo < hi) {
        idx_t mid = lo + (hi - lo) / 2;
        idx_t a   = isqrt_u128((__int128)2 * mid * mid);
        if (a < n) lo = mid + 1;
        else       hi = mid;
    }
    if (isqrt_u128((__int128)2 * lo * lo) == n) return lo;
    return 0;
}

/* Find k such that ⌊k·√2⌋ + 2k == n, or return 0 if none exists.  */
static idx_t find_k_by_second(idx_t n) {
    idx_t lo = 1, hi = n;          /* an upper bound is n       */
    while (lo < hi) {
        idx_t mid = lo + (hi - lo) / 2;
        idx_t a   = isqrt_u128((__int128)2 * mid * mid);
        idx_t s   = a + 2 * mid;
        if (s < n) lo = mid + 1;
        else       hi = mid;
    }
    if (isqrt_u128((__int128)2 * lo * lo) + 2 * lo == n) return lo;
    return 0;
}

seq_t a(idx_t n) {
    if (n == 0) return 0;          /* out of range, not used in OEIS */
    idx_t k = find_k_by_first(n);
    if (!k) k = find_k_by_second(n);
    if (!k) return 0;              /* safety fallback, should not happen */
    return (seq_t)((seq_t)2 * (seq_t)k);
}
