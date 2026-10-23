/*
 * A128282  Regular symmetric triangle, read by rows, whose left half consists
 *          of the positive integers.
 * https://oeis.org/A128282
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = start(r) + min(k, r - k) where r =
 *            floor((sqrt(8n+1)-1)/2) (row index) k = n - r(r+1)/2 (position
 *            inside the row) start(r) = 1 + { m(m+1) if r even (m+1)² if r
 *            odd } with m = floor(r/2). The tile is symmetric: left half
 *            increases by 1, right half mirrors
 * Range:     u8 n <= 77, u16 n <= 77, u32 n <= 77, u64 n <= 77, u128 n <= 77,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static inline idx_t isqrt_u128(__uint128_t x)
{
    idx_t lo = 0, hi = ((idx_t)1 << 63);        /* sqrt of 2^128 fits in 2^64 */
    while (lo < hi) {
        idx_t mid = (lo + hi + 1) >> 1;
        if (( (__uint128_t)mid * mid ) <= x)
            lo = mid;
        else
            hi = mid - 1;
    }
    return lo;
}

seq_t a(idx_t n)
{
    /* r = floor((sqrt(8n+1)-1)/2) */
    __uint128_t t = (__uint128_t)n * 8 + 1;
    idx_t s = isqrt_u128(t);
    idx_t r = (s - 1) >> 1;                    /* r = (s-1)/2 */

    /* row start index = r(r+1)/2 */
    __uint128_t rs = (__uint128_t)r * (r + 1) / 2;
    idx_t row_start = (idx_t)rs;

    idx_t offset = n - row_start;              /* k */

    /* start value of row r */
    idx_t m = r >> 1;                          /* floor(r/2) */
    seq_t start;
    if ((r & 1) == 0) {                       /* r even */
        __uint128_t prod = (__uint128_t)m * (m + 1);
        start = 1 + (seq_t)prod;
    } else {                                   /* r odd */
        __uint128_t prod = (__uint128_t)(m + 1) * (m + 1);
        start = 1 + (seq_t)prod;
    }

    /* value in the row */
    if (offset <= m) {
        return start + (seq_t)offset;          /* left part including center if odd */
    } else {
        return start + (seq_t)(r - offset);   /* right part (mirrored) */
    }
}
