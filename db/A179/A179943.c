/*
 * A179943  Triangle read by rows, antidiagonals of an array (r,k),
 *          r=(0,1,2,...), generated from 2 X 2 matrices of the form [1,r;
 *          1,(r+1)].
 * https://oeis.org/A179943
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = S_{p}(r+2) where * s = floor( (sqrt(8n+1) - 1) / 2 ) (s
 *            is the antidiagonal number) * p = n - s*(s+1)/2 (position within
 *            that diagonal) * r = s - p (row index) * S_0(x)=1, S_1(x)=x,
 *            S_m(x)=x*S_{m-1}(x)-S_{m-2}(x) (Chebyshev polynomials of the
 *            second kind)
 * Range:     u8 n <= 31, u16 n <= 71, u32 n <= 163, u64 n <= 423,
 *            u128 n <= 1113, big any n
 * Helpers:   chebyshev_s
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/chebyshev_s.h"

static inline idx_t triangular_index(idx_t n) {
    /* Find the largest s such that s*(s+1)/2 <= n.  */
    idx_t low = 0, high = n + 1, s = 0;
    while (low < high) {
        idx_t mid = low + (high - low) / 2;
        __uint128_t val = (__uint128_t)mid * (mid + 1) / 2;
        if (val <= n) {
            s = mid;
            low = mid + 1;
        } else {
            high = mid;
        }
    }
    return s;
}

seq_t a(idx_t n) {
    idx_t s = triangular_index(n);
    __uint128_t tri = (__uint128_t)s * (s + 1) / 2;
    idx_t p = n - (idx_t)tri;        /* position within the diagonal (0 ≤ p ≤ s) */
    idx_t r = s - p;                 /* row index                         */
    seq_t x = (seq_t)(r + 2);        /* x = r + 2                         */
    return chebyshev_s(p, x);
}
