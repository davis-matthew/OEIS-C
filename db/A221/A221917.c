/*
 * A221917  Difference between area/L^2 and perimeter/L, with some length unit
 *          L, of a rectangle n X m, n >= m >= 0.
 * https://oeis.org/A221917
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy, tabl
 * Formula:   a(n) = r*m - 2*(r + m), where r = floor((sqrt(8*n+1)-1)/2) and m
 *            = n - r*(r+1)/2 (enumerating rectangle rows in the OEIS order)
 * Range:     i8 n <= 65, i16 n <= 65, i32 n <= 65, i64 n <= 65, i128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Integer square root for 128‑bit numbers (floor). */
static idx_t isqrt_u128(__int128 w) {
    /* 8*n+1 <= 2^67 for n <= UINT64_MAX, so the sqrt fits in 35 bits. */
    idx_t low = 0;
    idx_t high = ((idx_t)1 << 35);   /* safe upper bound */
    while (low < high) {
        idx_t mid = (low + high) >> 1;
        __int128 mid2 = (__int128)mid * (__int128)mid;
        if (mid2 <= w)
            low = mid + 1;
        else
            high = mid;
    }
    return low - 1;   /* floor(sqrt(w)) */
}

seq_t a(idx_t n) {
    /* 1. Find row r such that r(r+1)/2 <= n < (r+1)(r+2)/2. */
    __int128 w = (__int128)8 * n + 1;
    idx_t s = isqrt_u128(w);
    idx_t row = (s - 1) >> 1;   /* floor((sqrt(8*n+1)-1)/2) */

    /* 2. Column index m. */
    __int128 start = (__int128)row * (row + 1) / 2;
    idx_t m = n - (idx_t)start;

    /* 3. Compute the value using 128‑bit intermediates. */
    __int128 prod = (__int128)row * (__int128)m;
    __int128 val128 = prod - (__int128)2 * ((__int128)row + (__int128)m);

    return (seq_t)val128;
}
