/*
 * A156297  Triangle T(n,k) read by rows. If n = k^2 then 1 else 0.
 * https://oeis.org/A156297
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   a(n) = 1 if the term n corresponds to a perfect-square row with
 *            column equal to its square root; otherwise 0
 * Range:     u8 n <= 9983, u16 n <= 9983, u32 n <= 9983, u64 n <= 9983,
 *            u128 n <= 9983, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static idx_t isqrt_u64(idx_t x) {
    idx_t lo = 0, hi = x;
    while (lo < hi) {
        idx_t mid = lo + (hi - lo + 1) / 2;
        if ( (__int128)mid * mid <= x )
            lo = mid;
        else
            hi = mid - 1;
    }
    return lo;
}

seq_t a(idx_t n) {
    /* find the row r such that r*(r+1)/2 >= n */
    idx_t r = (isqrt_u64((idx_t)( (__int128)8 * n + 1 )) - 1) / 2;
    if ( (__int128)r * (r + 1) / 2 < n )
        ++r;
    else
        while ( r > 0 && (__int128)(r - 1) * r / 2 >= n )
            --r;

    /* position k within the row (1‑based) */
    idx_t row_start = (__int128)(r - 1) * r / 2;
    idx_t k = n - row_start;

    /* check if r is a perfect square and k equals its root */
    idx_t s = isqrt_u64(r);
    if (s * s == r && k == s)
        return (seq_t)1;
    else
        return (seq_t)0;
}
