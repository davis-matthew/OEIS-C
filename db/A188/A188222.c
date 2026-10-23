/*
 * A188222  Beatty sequence for (sqrt(5) + 3)/4.
 * https://oeis.org/A188222
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor(n*(sqrt(5)+3)/4)
 * Range:     u8 n <= 168, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Integer square root for an unsigned __int128 value.
   Returns floor(sqrt(v)).  Uses only integer operations. */
static seq_t isqrt_u128(__uint128_t v)
{
    __uint128_t low = 0, high = v;
    while (low <= high) {
        __uint128_t mid = (low + high) >> 1;
        __uint128_t sq  = mid * mid;
        if (sq == v) return (seq_t)mid;
        if (sq < v)
            low = mid + 1;
        else
            high = mid - 1;
    }
    return (seq_t)high;
}

/* Beatty sequence for (sqrt(5)+3)/4.  Uses only integer arithmetic. */
seq_t a(idx_t n)
{
    /* Compute floor(n*sqrt(5)) as floor_sqrt(5*n*n). */
    __uint128_t prod = (__uint128_t)n * n * 5ULL;
    seq_t sqrt5n = isqrt_u128(prod);

    /* a(n) = floor( (sqrt5n + 3*n) / 4 ).  All arithmetic is integer. */
    __uint128_t numer = (__uint128_t)sqrt5n + (__uint128_t)3 * n;
    return (seq_t)(numer / 4);
}
