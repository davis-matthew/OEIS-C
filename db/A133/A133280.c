/*
 * A133280  Triangle formed by: 1 even, 2 odd, 3 even, 4 odd, ... starting
 *          with zero.
 * https://oeis.org/A133280
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, tabl
 * Formula:   a(n) = r^2 + 2*(n - r(r+1)/2); where r =
 *            floor((sqrt(8*n+1)-1)/2) gives the row index
 * Range:     u8 n <= 135, u16 n <= 5150, u32 n <= 5150, u64 n <= 5150,
 *            u128 n <= 5150, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    /* find row r such that r(r+1)/2 <= n < (r+1)(r+2)/2 */
    idx_t lo = 0;
    idx_t hi = ((idx_t)1 << 33);          /* upper bound > sqrt(2*2^64) */
    while (lo < hi) {
        idx_t mid = lo + (hi - lo + 1) / 2;
        __int128 val = (__int128)mid * (mid + 1) / 2;
        if (val <= n) {
            lo = mid;
        } else {
            hi = mid - 1;
        }
    }
    idx_t r = lo;
    __int128 cum_prev = (__int128)r * (r + 1) / 2;
    idx_t offset = n - (idx_t)cum_prev;

    return (seq_t)r * (seq_t)r + (seq_t)2 * (seq_t)offset;
}
