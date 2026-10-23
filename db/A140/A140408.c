/*
 * A140408  Irregular triangle T(n,k) read by rows: n repetitions of -1
 *          followed by (n+1) repetitions of n+1.
 * https://oeis.org/A140408
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, tabf, easy
 * Formula:   a(n) = -1 if n < r^2 + r else r+1, where r = floor(sqrt(n))
 * Range:     i8 n <= 99, i16 n <= 99, i32 n <= 99, i64 n <= 99, i128 n <= 99,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Integer square root for uint64_t, uses binary search over [0,2^32] */
static uint64_t isqrt_u64(uint64_t n)
{
    uint64_t low = 0;
    uint64_t high = (n < 4294967295ULL) ? n + 1 : 4294967296ULL; /* 2^32 */
    while (low + 1 < high) {
        uint64_t mid = low + (high - low) / 2;
        __uint128_t sq = (__uint128_t)mid * mid;
        if (sq <= n) {
            low = mid;
        } else {
            high = mid;
        }
    }
    return low;
}

seq_t a(idx_t n)
{
    uint64_t m = (uint64_t)n;
    uint64_t r = isqrt_u64(m);                      /* row index */
    uint64_t start = (uint64_t)((__uint128_t)r * r);  /* first index of this row */
    uint64_t pos = m - start;                       /* position within the row */

    seq_t r1 = (seq_t)r;
    return (pos < r) ? (seq_t)-1 : r1 + (seq_t)1;
}
