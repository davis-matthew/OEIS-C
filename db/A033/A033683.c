/*
 * A033683  a(n) = 1 if n is an odd square not divisible by 3, otherwise 0.
 * https://oeis.org/A033683
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, mult, easy
 * Formula:   a(n) = 1 if n is an odd square not divisible by 3, otherwise 0
 * Range:     u8 n <= 8974, u16 n <= 8974, u32 n <= 8974, u64 n <= 8974,
 *            u128 n <= 8974, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static inline idx_t _isqrt_u64(idx_t n)
{
    /* Binary search for floor(sqrt(n)) */
    idx_t lo = 0, hi = (idx_t)1ULL << 32;  /* hi > sqrt(UINT64_MAX) */
    while (lo + 1 < hi) {
        idx_t mid = lo + (hi - lo) / 2;
        if (mid <= n / mid)   /* mid*mid <= n, avoid overflow */
            lo = mid;
        else
            hi = mid;
    }
    return lo;
}

seq_t a(idx_t n)
{
    idx_t s = _isqrt_u64(n);
    if (s * s != n)               /* not a perfect square */
        return (seq_t)0;
    if ((s & 1) == 0)              /* even square root => even square */
        return (seq_t)0;
    if ((n % 3) == 0)              /* divisible by 3 */
        return (seq_t)0;
    return (seq_t)1;               /* odd square not divisible by 3 */
}
