/*
 * A111651  n appears 3n times.
 * https://oeis.org/A111651
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = round(sqrt((2/3)*n))
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Integer square root for unsigned __int128 values.
   Returns floor(sqrt(val)). */
static uint64_t isqrt128(unsigned __int128 val) {
    uint64_t lo = 0, hi = ((uint64_t)1) << 35; /* > sqrt(6*UINT64_MAX) */
    while (lo < hi) {
        uint64_t mid = (lo + hi + 1) >> 1;
        unsigned __int128 sq = (unsigned __int128)mid * (unsigned __int128)mid;
        if (sq <= val) {
            lo = mid;
        } else {
            hi = mid - 1;
        }
    }
    return lo;
}

seq_t a(idx_t n) {
    /* for offset 1, n>=1 */
    if (n == 0) return (seq_t)0;

    /* Compute sqrt(6*n) exactly */
    unsigned __int128 prod = (unsigned __int128)6 * (unsigned __int128)n;
    uint64_t s = isqrt128(prod);          /* floor(sqrt(6*n)) */
    uint64_t k = s / 3;                   /* floor(sqrt(6*n)/3) */

    /* Decide whether to round up or not:
       round(sqrt(6*n)/3) = k  if 24*n < 36*k^2 + 36*k + 9
                           = k+1 otherwise */
    unsigned __int128 left  = (unsigned __int128)24 * (unsigned __int128)n;
    unsigned __int128 right = (unsigned __int128)36 * (unsigned __int128)k * k
                           + (unsigned __int128)36 * (unsigned __int128)k
                           + 9;

    if (left >= right) ++k;

    return (seq_t)k;
}
