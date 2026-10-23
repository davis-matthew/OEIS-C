/*
 * A104136  a(n) = ceiling(sqrt(2*Pi*n)).
 * https://oeis.org/A104136
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ceil(sqrt(2*Pi*n))
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static const unsigned long long SCALE   = 1000000000000000ULL; /* 10^15 */
static const unsigned long long PI_SCALER = 3141592653589793ULL; /* round(pi*10^15) */

/* Compute a(n) = ceiling(sqrt(2*Pi*n)) using only integer arithmetic.
   All intermediate calculations are performed in unsigned __int128 */
seq_t a(idx_t n)
{
    /* For n < 1 the behavior is unspecified; return 0. */
    if (n < 1)
        return 0;

    /* num = 2 * PI_SCALER * n ;   (numerator of the scaled value) */
    unsigned __int128 num = (__int128)(2ULL * PI_SCALER) * (unsigned __int128)n;

    /* Binary search for floor(sqrt(num / SCALE)) */
    unsigned __int128 low  = 0, high = 1;

    /* Find an upper bound: while (high^2 * SCALE <= num) double it. */
    while (high * high * SCALE <= num)
        high <<= 1;

    /* Binary search between low and high (inclusive) for the largest k
       such that k^2 * SCALE <= num. */
    while (low < high) {
        unsigned __int128 mid = (low + high + 1) >> 1;
        if (mid * mid * SCALE <= num)
            low = mid;
        else
            high = mid - 1;
    }

    /* low is floor(sqrt(num / SCALE)).  The desired sequence value is
       ceil(sqrt(2*Pi*n)) = low + 1. */
    return (seq_t)(low + 1);
}
