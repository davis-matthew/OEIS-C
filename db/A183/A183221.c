/*
 * A183221  Complement of the 9-gonal numbers.
 * https://oeis.org/A183221
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n + floor(1/2 + sqrt(2n/7))
 * Range:     u8 n <= 100, u16 n <= 100, u32 n <= 100, u64 n <= 100,
 *            u128 n <= 100, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Integer square root for unsigned 64‑bit values.
   Returns floor(sqrt(x)). */
static idx_t isqrt_u64(idx_t x)
{
    idx_t low = 0;
    idx_t high = x;
    while (low <= high) {
        idx_t mid = low + ((high - low) >> 1);
        __uint128_t sq = (__uint128_t)mid * mid;
        if (sq == x) {
            return mid;
        } else if (sq < x) {
            low = mid + 1;
        } else {
            if (mid == 0) break;
            high = mid - 1;
        }
    }
    return high;   /* floor(sqrt(x)) */
}

/* Sequence generator for A183221: Complement of the 9‑gonal numbers. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined for offset 1, but harmless */
    /* Compute floor( sqrt( (8n) / 7 ) ) */
    __uint128_t mul = (__uint128_t)n * 8;
    idx_t tmp = (idx_t)(mul / 7);
    idx_t t   = isqrt_u64(tmp);
    /* k = floor( (t + 1) / 2 )  == floor( sqrt(2n/7) + 0.5 ) */
    idx_t k   = (t + 1) >> 1;
    return (seq_t)n + (seq_t)k;
}
