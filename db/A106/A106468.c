/*
 * A106468  Absolute value of inverse of number triangle A106465.
 * https://oeis.org/A106468
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = entry of triangle A106465 flattened in row-major order.
 *            Triangle definition: even column j = 2k -> 1 at rows r = 2k,
 *            2k+1, 2k+2, 2k+3; odd column j = 2k+1 -> 1 at rows r = 2k+1,
 *            2k+3. Hence a(n) = 1 if the corresponding triangle entry is 1,
 *            else 0. This implementation uses only integer ari...
 * Range:     u8 n <= 104, u16 n <= 104, u32 n <= 104, u64 n <= 104,
 *            u128 n <= 104, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static inline uint64_t isqrt_uint128(__int128 v) {
    uint64_t lo = 0, hi = (uint64_t)1 << 34; /* upper bound = 2^34 > sqrt(2^66) */
    while (lo < hi) {
        uint64_t mid = (lo + hi + 1) >> 1;
        if ((__int128)mid * mid <= v)
            lo = mid;
        else
            hi = mid - 1;
    }
    return lo;
}

seq_t a(idx_t n) {
    /* Find row r such that r(r+1)/2 ≤ n < (r+1)(r+2)/2. */
    __int128 z = (__int128)n * 8 + 1;
    uint64_t s = isqrt_uint128(z);
    uint64_t r = (s - 1) >> 1;          /* floor((sqrt(8n+1)-1)/2) */

    /* Compute row start index (first element of row r). */
    __int128 start128 = (__int128)r * (r + 1) / 2;
    uint64_t start = (uint64_t)start128;

    /* Column c (0 ≤ c ≤ r). */
    uint64_t c = n - start;

    /* Determine if the triangle entry is 1. */
    if ((c & 1) == 0) {                 /* even column */
        return (r <= c + 3) ? (seq_t)1 : (seq_t)0;
    } else {                             /* odd column */
        return ((r == c) || (r == c + 2)) ? (seq_t)1 : (seq_t)0;
    }
}
