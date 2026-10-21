/*
 * A219191  Numbers of the form k*(7*k+1), where k = 0,-1,1,-2,2,-3,3,...
 * https://oeis.org/A219191
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (14*n*(n-1)+5*(-1)^n*(2*n-1)+5)/8
 * Range:     u8 n <= 12, u16 n <= 193, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n <= 1)            /* a(1) = 0 */
        return 0;

    /* use unsigned 128‑bit arithmetic to avoid overflow */
    __uint128_t N   = n;
    __uint128_t num = 14ULL * N * (N - 1);   /* 14*n*(n-1) */

    if ((n & 1) == 0) { /* n even:  +5*(2n-1)+5 */
        num += 5ULL * (2 * N - 1) + 5ULL;
    } else {            /* n odd:   -5*(2n-1)+5 */
        num -= 5ULL * (2 * N - 1);
        num += 5ULL;
    }

    return (seq_t)(num / 8);   /* division is exact */
}
