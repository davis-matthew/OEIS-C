/*
 * A190762  [(bn+c)r]-b[nr]-[cr], where (r,b,c)=(sqrt(1/2),2,1) and [ ]=floor.
 * https://oeis.org/A190762
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor((bn+c)r) - b*floor(nr) - floor(cr) with
 *            (r,b,c)=(sqrt(1/2),2,1)
 * Range:     u8 n <= 132, u16 n <= 132, u32 n <= 132, u64 n <= 132,
 *            u128 n <= 132, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Integer square root of an unsigned 128‑bit value.
   Returns floor( sqrt(x) ). */
static inline uint64_t isqrt_u128(__uint128_t x)
{
    uint64_t r = 0;
    uint64_t bit = (uint64_t)1ULL << 63; /* highest possible bit */
    /* Shift bit down until bit*bit <= x */
    while (bit != 0 && (__uint128_t)bit * bit > x)
        bit >>= 1;
    while (bit != 0) {
        uint64_t temp = r + bit;
        if ((__uint128_t)temp * temp <= x)
            r = temp;
        bit >>= 1;
    }
    return r;
}

seq_t a(idx_t n)
{
    /* n >= 1 */
    uint64_t N = n;
    /* compute floor((2N+1)*sqrt(2)) = floor( sqrt( 2*(2N+1)^2 ) ) */
    __uint128_t t1 = (__uint128_t)(2 * N + 1);
    t1 = t1 * t1 * 2;
    uint64_t a1 = isqrt_u128(t1);

    /* compute floor(N*sqrt(2)) = floor( sqrt( 2*N^2 ) ) */
    __uint128_t t2 = (__uint128_t)N * N * 2;
    uint64_t b1 = isqrt_u128(t2);

    /* a(n) = floor((2N+1)*sqrt(2)/2) - 2*floor(N*sqrt(2)/2) */
    uint64_t result = (a1 / 2) - 2 * (b1 / 2);
    return (seq_t)result;
}
