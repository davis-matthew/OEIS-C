/*
 * A187976  a(n) = [nr+kr]-[nr]-[kr], where r=sqrt(2), k=6, [ ]=floor.
 * https://oeis.org/A187976
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A187976
   a(n) = floor((n+6)*sqrt(2)) – floor(n*sqrt(2)) – 8
   (floor(6*sqrt(2)) = 8)                 */
#include <stdint.h>

/* Integer square root of an unsigned __int128.
   Returns floor(sqrt(x)). */
static inline seq_t isqrt_uint128(__uint128_t x)
{
    unsigned __int128 r = 0;
    unsigned __int128 bit = (__uint128_t)1 << 126; /* largest power of four <= 2^127 */
    while (bit > x) bit >>= 2;
    while (bit != 0) {
        if (x >= r + bit) {
            x -= r + bit;
            r = (r >> 1) + bit;
        } else {
            r >>= 1;
        }
        bit >>= 2;
    }
    return (seq_t)r;
}

/* floor(n * sqrt(2)) computed with integer arithmetic. */
static inline seq_t floor_n_sqrt2(idx_t n)
{
    __uint128_t val = (__uint128_t)2 * n * n; /* 2*n^2 fits in 128 bits for all practical n */
    return isqrt_uint128(val);
}

seq_t a(idx_t n)
{
    /* a(n) = floor((n+6)*sqrt2) – floor(n*sqrt2) – 8 */
    seq_t t1 = floor_n_sqrt2(n + 6);
    seq_t t2 = floor_n_sqrt2(n);
    return t1 - t2 - (seq_t)8;
}
