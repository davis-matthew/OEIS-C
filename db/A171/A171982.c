/*
 * A171982  Beatty sequence for sqrt(11).
 * https://oeis.org/A171982
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor(n*sqrt(11))
 * Range:     u8 n <= 77, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Integer square root of a 128‑bit number.
 * Returns floor(sqrt(x)). */
static unsigned long long sqrt_u128(__uint128_t x)
{
    unsigned long long low  = 0;
    unsigned long long high = 0xFFFFFFFFFFFFFFFFULL; /* 2^64 - 1 */

    while (low <= high) {
        unsigned long long mid = low + ((high - low) >> 1);
        __uint128_t mid2 = (__uint128_t)mid * mid;

        if (mid2 == x)
            return mid;
        if (mid2 < x)
            low = mid + 1;
        else
            high = mid - 1;
    }
    /* high < low, and high is floor(sqrt(x)) */
    return high;
}

seq_t a(idx_t n)
{
    if (n == 0)           /* Offset is 1, but handle 0 just in case */
        return (seq_t)0;

    /* Compute floor(n * sqrt(11)) as floor(sqrt(11 * n^2)) */
    __uint128_t prod = (__uint128_t)n * n * 11;
    unsigned long long root = sqrt_u128(prod);

    return (seq_t)root;
}
