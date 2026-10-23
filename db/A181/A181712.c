/*
 * A181712  a(n) = floor(3*n*phi) - floor(2*n*phi) - floor(n*phi), where phi =
 *          (1+sqrt(5))/2, the golden ratio.
 * https://oeis.org/A181712
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor(3*n*phi) - floor(2*n*phi) - floor(n*phi)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Assuming phi = (1+sqrt(5))/2. 
 * The computation proceeds by expressing each floor in terms of
 * floor(n*sqrt(5)) and floor(3n*sqrt(5)), which are obtained by an
 * integer square‑root. No floating point or math.h functions are used. */
#include <stdint.h>

/* Integer square root for __uint128_t: returns floor(sqrt(x)) */
static inline __uint128_t u128_sqrt(__uint128_t x)
{
    __uint128_t lo = 0, hi = x, mid, ans = 0;
    while (lo <= hi) {
        mid = (lo + hi) >> 1;
        __uint128_t sq = mid * mid;
        if (sq <= x) {
            ans = mid;
            lo = mid + 1;
        } else {
            if (mid == 0) break;
            hi = mid - 1;
        }
    }
    return ans;
}

seq_t a(idx_t n)
{
    /* Promote index to a wider type for intermediate arithmetic */
    __uint128_t nn = (__uint128_t)n;

    /* floor(n*sqrt(5))  -> f   */
    __uint128_t v5 = 5 * nn * nn;
    __uint128_t f  = u128_sqrt(v5);

    /* floor(3n*sqrt(5)) -> f3  */
    __uint128_t v45 = 45 * nn * nn;
    __uint128_t f3 = u128_sqrt(v45);

    /* w1 = floor((3n + f3)/2) */
    __uint128_t w1 = (3 * nn + f3) >> 1;

    /* w2 = n + f (exact integer) */
    __uint128_t w2 = nn + f;

    /* w3 = floor((n + f)/2) */
    __uint128_t w3 = (nn + f) >> 1;

    /* Result = w1 - w2 - w3 */
    __uint128_t res128 = w1 - w2 - w3;

    return (seq_t)res128;
}
