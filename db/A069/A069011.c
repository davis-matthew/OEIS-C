/*
 * A069011  Triangle with T(n,k) = n^2 + k^2.
 * https://oeis.org/A069011
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, tabl
 * Formula:   a(n) = T(r,k) where r = floor((sqrt(1+8*n)-1)/2) k = n -
 *            r*(r+1)/2 T(r,k) = r^2 + k^2 (OEIS A069011, triangle
 *            T(n,k)=n^2+k^2)
 * Range:     u8 n <= 88, u16 n <= 7380, u32 n <= 7380, u64 n <= 7380,
 *            u128 n <= 7380, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

/* integer square root of a 64‑bit unsigned integer (floor sqrt) */
static uint64_t isqrt_u64(uint64_t x)
{
    uint64_t lo = 0, hi = x < 0x8000000000000000ULL ? x : 0x8000000000000000ULL;
    while (lo <= hi) {
        uint64_t mid = lo + ((hi - lo) >> 1);
        __uint128_t sq = (__uint128_t)mid * mid;
        if (sq <= x)
            lo = mid + 1;
        else
            hi = mid - 1;
    }
    return hi;
}

/* integer square root of a 128‑bit unsigned integer (floor sqrt) */
static uint64_t isqrt_u128(__uint128_t x)
{
    uint64_t lo = 0, hi = (uint64_t)0xFFFFFFFFFFFFFFFFULL;
    while (lo <= hi) {
        uint64_t mid = lo + ((hi - lo) >> 1);
        __uint128_t sq = (__uint128_t)mid * mid;
        if (sq <= x)
            lo = mid + 1;
        else
            hi = mid - 1;
    }
    return hi;
}

/* main sequence function */
seq_t a(idx_t n)
{
    /* compute r = floor((sqrt(1+8*n)-1)/2) safely for large n */
    __uint128_t val = (__uint128_t)8 * n + 1;
    uint64_t sqrt_val = isqrt_u128(val);
    idx_t r = (idx_t)((sqrt_val - 1) / 2);

    /* offset within the row */
    idx_t prefix = r * (r + 1) / 2;            /* terms before row r */
    idx_t k = n - prefix;                      /* column index */

    /* compute r^2 + k^2 in seq_t */
    seq_t rr = (seq_t)r * (seq_t)r;
    seq_t kk = (seq_t)k * (seq_t)k;
    return rr + kk;
}
