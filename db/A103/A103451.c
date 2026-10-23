/*
 * A103451  Triangular array T read by rows: T(n, 0) = T(n, n) = 1, T(n, k) =
 *          0 for 0 < k < n.
 * https://oeis.org/A103451
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, tabl
 * Formula:   a(n) = 1 if n = r(r+1)/2 for some r ≥ 0 or n = r(r+3)/2 for some
 *            r ≥ 0 a(n) = 0 otherwise (Triangular skeleton of Pascal’s
 *            triangle read by rows.)
 * Range:     u8 n <= 9901, u16 n <= 9901, u32 n <= 9901, u64 n <= 9901,
 *            u128 n <= 9901, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* integer square root of 128‑bit unsigned integer, floor value */
static inline unsigned long long isqrt128(unsigned __int128 x) {
    unsigned long long lo = 0, hi = 1ULL << 63; /* hi^2 fits in 128 bits */
    while (lo < hi) {
        unsigned long long mid = (lo + hi + 1) >> 1;
        if ((unsigned __int128)mid * mid <= x)
            lo = mid;
        else
            hi = mid - 1;
    }
    return lo;
}

seq_t a(idx_t n) {
    /* Check n = r(r+1)/2 */
    unsigned __int128 x1 = (unsigned __int128)n * 8 + 1;
    unsigned long long s1 = isqrt128(x1);
    unsigned long long r1 = (s1 - 1) >> 1;
    if ((unsigned __int128)r1 * (r1 + 1) / 2 == n)
        return 1;

    /* Check n = r(r+3)/2 */
    unsigned __int128 x2 = (unsigned __int128)n * 8 + 9;
    unsigned long long s2 = isqrt128(x2);
    if (s2 >= 3 && (s2 - 3) % 2 == 0) {
        unsigned long long r2 = (s2 - 3) >> 1;
        if ((unsigned __int128)r2 * (r2 + 3) / 2 == n)
            return 1;
    }

    return 0;
}
