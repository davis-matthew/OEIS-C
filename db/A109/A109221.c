/*
 * A109221  A number triangle related to the Fibonacci polynomials.
 * https://oeis.org/A109221
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 71, u16 n <= 81, u32 n <= 81, u64 n <= 81, u128 n <= 81,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Sequence A109221: triangle T(n,k) enumerated in row-major order.
 * Formula for the triangle entry: T(n,k)=binomial(floor((2n-k)/2)+n-k, 2n-2k).
 * Index mapping: given global index i, let r be the row with r(r+1)/2 ≤ i < (r+1)(r+2)/2,
 * and let c = i - r(r+1)/2. Then a(i)=T(r,c).
 */

#include "../utils/binomial.h"

seq_t a(idx_t n) {
    /* Find row r such that r(r+1)/2 ≤ n < (r+1)(r+2)/2 */
    idx_t lo = 0, hi = 1;
    while ( ( (__int128)hi * (hi + 1) ) / 2 <= n ) {
        hi <<= 1;
    }
    while (lo + 1 < hi) {
        idx_t mid = lo + ((hi - lo) >> 1);
        if ( ( (__int128)mid * (mid + 1) ) / 2 <= n ) {
            lo = mid;
        } else {
            hi = mid;
        }
    }
    idx_t r = lo;
    idx_t offset = (idx_t)(( (__int128)r * (r + 1) ) / 2);
    idx_t c = n - offset;

    /* Compute parameters of the binomial coefficient */
    __int128 two_r_minus_c = (__int128)2 * r - c;
    idx_t a_val = (idx_t)( two_r_minus_c / 2 + ( (__int128)r - c ) );
    __int128 k_temp = (__int128)2 * r - 2 * c;
    idx_t k_val = (idx_t)k_temp;

    return binomial(a_val, k_val);
}
