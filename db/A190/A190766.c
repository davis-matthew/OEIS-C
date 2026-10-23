/*
 * A190766  [(bn+c)r]-b[nr]-[cr], where (r,b,c)=(sqrt(1/2),3,0) and [ ]=floor.
 * https://oeis.org/A190766
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor((3n)r) - 3*floor(n r) with r = sqrt(1/2) =
 *            1/sqrt(2) = sqrt(2)/2. Using the identity floor(n/√2) = floor(
 *            sqrt(2 n^2) ) / 2 we rewrite a(n) = floor( sqrt(18 n^2) ) / 2 -
 *            3 * floor( sqrt(2 n^2) ) / 2
 * Range:     u8 n <= 132, u16 n <= 132, u32 n <= 132, u64 n <= 132,
 *            u128 n <= 132, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This uses only integer arithmetic and a 64‑bit integer square root. */

#include <stdint.h>

/* Integer square root (floor).  Works for 0 <= x < 2^64. */
static inline uint64_t isqrt_u64(uint64_t x) {
    uint64_t r = 0;
    uint64_t bit = 1ULL << 62;          /* largest power of 4 <= x */
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
    return r;
}

seq_t a(idx_t n) {
    /* protect against the defined offset: n >= 1 */
    uint64_t N = (uint64_t)n;

    /* Compute 18 * n^2 and 2 * n^2 using 128‑bit to avoid overflow. */
    uint64_t t1 = (unsigned __int128)N * N * 18;
    uint64_t t2 = (unsigned __int128)N * N * 2;

    /* floor(sqrt(18 n^2)) / 2  and  floor(sqrt(2 n^2)) / 2 */
    uint64_t s1 = isqrt_u64(t1) >> 1;   /* divide by 2 using shift */
    uint64_t s2 = isqrt_u64(t2) >> 1;

    uint64_t result = s1 - 3 * s2;
    return (seq_t)result;
}
