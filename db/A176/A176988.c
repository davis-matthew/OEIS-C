/*
 * A176988  Triangle read by rows, which contains Noll's indices of Zernike
 *          polynomials in row n sorted along increasing index of the
 *          azimuthal quantum number.
 * https://oeis.org/A176988
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, tabl
 * Formula:   a(n) = T(r,k) where r = largest integer with r*(r+1)/2 <= n, k =
 *            n - r*(r+1)/2, T(r,k) = r*(r+1)/2 + abs(m) + h, m = 2*n -
 *            r*(r+2), h = 1 if (r%4<=1 && m<=0) || (r%4>1 && m>=0), else 0
 * Range:     u8 n <= 119, u16 n <= 119, u32 n <= 119, u64 n <= 119,
 *            u128 n <= 119, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <math.h>
#include <stdint.h>
#include <stdbool.h>

/* Integer triangular number in 128‑bit to avoid overflow. */
static inline __int128 tri128(uint64_t x) {
    return (__int128)x * (x + 1) / 2;
}

seq_t a(idx_t n) {
    /* --- find row r such that tri(r) <= n < tri(r+1) --- */
    long double t = sqrtl((long double)(8.0L * (long double)n + 1.0L));
    uint64_t r = (uint64_t)((t - 1.0L) / 2.0L);
    while (tri128(r) > ( __int128)n) r--;
    while (tri128(r + 1) <= ( __int128)n) r++;

    /* --- offset inside the row --- */
    uint64_t tri = (uint64_t)(tri128(r));
    uint64_t k = n - tri;          /* 0 <= k <= r */

    /* --- compute m and h --- */
    int64_t m  = (int64_t)(2 * (int64_t)n - (int64_t)r * (r + 2));
    int64_t abs_m = m < 0 ? -m : m;
    int h = 0;
    if ((r % 4 <= 1 && m <= 0) || (r % 4 > 1 && m >= 0))
        h = 1;

    /* --- final value --- */
    seq_t result = (seq_t)((seq_t)r * (r + 1) / 2 + abs_m + h);
    return result;
}
