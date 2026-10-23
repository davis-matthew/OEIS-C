/*
 * A115356  Matrix (1,x)+(x,x^2) in Riordan array notation.
 * https://oeis.org/A115356
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, tabl
 * Formula:   a(n) = 1 if there exist non‑negative integers r,k such that * n
 *            = r(r+3)/2 (the “last element” of row r) or * n = 2k^2+4k+1 (the
 *            “middle element” of odd rows); * otherwise a(n) = 0. * This
 *            matches the triangle T(n,k) defined by * T(n,k) = 1 if n = k or
 *            n = 2k+1, 0 otherwise
 * Range:     u8 n <= 9143, u16 n <= 9143, u32 n <= 9143, u64 n <= 9143,
 *            u128 n <= 9143, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Integer square root of a 128‑bit unsigned integer.
 * Returns floor(sqrt(x)).  Uses binary search. */
static inline uint64_t isqrt_u128(unsigned __int128 x) {
    uint64_t lo = 0, hi = UINT64_MAX, mid;
    while (lo <= hi) {
        mid = lo + ((hi - lo) >> 1);
        unsigned __int128 sq = (unsigned __int128)mid * mid;
        if (sq <= x)
            lo = mid + 1;
        else
            hi = mid - 1;
    }
    return hi;
}

seq_t a(idx_t n) {
    /* Check pattern n = r(r+3)/2  */
    unsigned __int128 D1 = (unsigned __int128)9 + (unsigned __int128)8 * n;
    uint64_t s1 = isqrt_u128(D1);
    if ((unsigned __int128)s1 * s1 == D1) {
        if (s1 >= 3 && ((s1 - 3) & 1ULL) == 0ULL) {
            /* r = (s1 - 3)/2 is non‑negative integer, so n matches the
               first condition.  No further check needed because the
               discriminant guarantee is sufficient. */
            return (seq_t)1;
        }
    }

    /* Check pattern n = 2k^2 + 4k + 1  */
    unsigned __int128 D2 = (unsigned __int128)8 * (n + 1);
    uint64_t s2 = isqrt_u128(D2);
    if ((unsigned __int128)s2 * s2 == D2) {
        if (s2 >= 4 && ((s2 - 4) % 4ULL) == 0ULL) {
            /* k = (s2 - 4)/4 is non‑negative integer, so n matches the
               second condition. */
            return (seq_t)1;
        }
    }

    return (seq_t)0;
}
