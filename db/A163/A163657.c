/*
 * A163657  Triangle T(m,n) = 2*m*n + m + n + 8 read by rows.
 * https://oeis.org/A163657
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, tabl
 * Formula:   a(n) = 2*m*j + m + j + 8 where m = ceil((sqrt(1 + 8*n) - 1)/2) j
 *            = n - (m-1)*m/2 T(m,j) = 2*m*j + m + j + 8 on the triangle read
 *            by rows
 * Range:     u8 n <= 65, u16 n <= 5050, u32 n <= 5050, u64 n <= 5050,
 *            u128 n <= 5050, big any n
 * Helpers:   isqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/isqrt.h"  // provides _floor_isqrt_u64

seq_t a(idx_t n) {
    if (n == 0) return 0;        // undefined for offset < 1, return 0

    /* Binary search for row m such that m*(m+1)/2 >= n */
    idx_t low = 1, high = n;
    while (low < high) {
        idx_t mid = low + (high - low) / 2;
        __uint128_t tri = (__uint128_t)mid * (mid + 1) / 2;
        if (tri >= n)
            high = mid;
        else
            low = mid + 1;
    }
    idx_t m = low;

    /* Compute the index j within row m */
    __uint128_t tprev128 = (__uint128_t)(m - 1) * m / 2;
    idx_t tprev = (idx_t)tprev128;
    idx_t j = n - tprev;          // 1 <= j <= m

    /* Compute the triangle value using 128‑bit intermediate to avoid overflow */
    __uint128_t val128 = (__uint128_t)2 * m * j + m + j + 8;
    return (seq_t)val128;
}
