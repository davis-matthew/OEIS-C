/*
 * A220073  Mirror of the triangle A130517.
 * https://oeis.org/A220073
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 1146, u16 n <= 7260, u32 n <= 7260, u64 n <= 7260,
 *            u128 n <= 7260, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = |(t+1)^2 - 2*n| + floor((t^2 + 3*t + 2 - 2*n)/(t+1)),
 * where t = floor((sqrt(8*n - 7) - 1)/2).
 */

#include <stdint.h>

/* Integer square root of an unsigned __int128 value. */
static inline idx_t isqrt_u128(unsigned __int128 x)
{
    unsigned __int128 low = 0, high = x;
    while (low <= high) {
        unsigned __int128 mid = (low + high) >> 1;
        unsigned __int128 sq  = mid * mid;
        if (sq <= x) {
            low = mid + 1;
        } else {
            high = mid - 1;
        }
    }
    return (idx_t)high;  /* result fits in 64 bits (sqrt<2^64) */
}

seq_t a(idx_t n)
{
    /* Compute t = floor((sqrt(8*n - 7) - 1)/2) */
    unsigned __int128 val = (unsigned __int128)8 * n - 7;
    idx_t s = isqrt_u128(val);
    idx_t t = (s - 1) / 2;

    /* (t+1)^2 */
    seq_t t1   = (seq_t)(t + 1);
    seq_t t1sq = t1 * t1;

    /* 2*n */
    seq_t two_n = (seq_t)n * 2;

    /* absolute difference */
    seq_t diff = (t1sq > two_n) ? (t1sq - two_n) : (two_n - t1sq);

    /* numerator for the floor division */
    seq_t num = (seq_t)t * (seq_t)t + (seq_t)3 * t + 2 - two_n;

    /* denominator (t+1) */
    seq_t den = t1;

    /* floor division (numerator non‑negative) */
    seq_t div = num / den;

    return diff + div;
}
