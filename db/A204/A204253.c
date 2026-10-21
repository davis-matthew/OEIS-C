/*
 * A204253  Symmetric matrix given by f(i,j)=1+[(i+j) mod 3].
 * https://oeis.org/A204253
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = 1 + ((d(n)+1) mod 3) where d(n) is the minimal integer
 *            satisfying d(d+1)/2 ≥ n (i.e. the diagonal index in the
 *            antidiagonal enumeration of the symmetric matrix f(i,j) = 1 +
 *            [(i+j) mod 3]). The sequence thus consists of blocks of constant
 *            values: 3 once, 1 twice, 2 thrice, 3 four times, 1 f...
 * Range:     u8 n <= 99, u16 n <= 99, u32 n <= 99, u64 n <= 99, u128 n <= 99,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Integer square root for a 128‑bit unsigned value.
   Returns floor(sqrt(x)). */
static uint64_t sqrt_u128(__uint128_t x)
{
    /* Upper bound: sqrt(2^68) ≈ 2^34 = 17179869184 > sqrt(max 1+8*n). */
    uint64_t lo = 0;
    uint64_t hi = (uint64_t)1 << 34;          /* 17,179,869,184   */
    while (lo <= hi) {
        uint64_t mid = lo + ((hi - lo) >> 1);
        __uint128_t mid2 = (__uint128_t)mid * mid;
        if (mid2 == x)
            return mid;
        if (mid2 < x)
            lo = mid + 1;
        else
            hi = mid - 1;
    }
    return hi;   /* hi < lo, hi is floor(sqrt(x)) */
}

seq_t a(idx_t n)
{
    /* n ≥ 1 (offset starts at 1) */
    /* Find the smallest d such that d(d+1)/2 ≥ n. */
    __uint128_t val = (__uint128_t)1 + (__uint128_t)8 * n;     /* 1 + 8*n fits in 128 bits */
    uint64_t sqrt_val = sqrt_u128(val);                      /* floor(sqrt(1+8*n))   */
    uint64_t d = (sqrt_val - 1) >> 1;                         /* floor((sqrt-1)/2)    */

    /* Correct d if necessary (since we used floor). */
    while (((__uint128_t)d * (d + 1) >> 1) < n)
        d++;

    /* The value on diagonal d is 1 + ((d+1) mod 3). */
    uint64_t v = 1 + ((d + 1) % 3);
    return (seq_t)v;
}
