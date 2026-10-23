/*
 * A164514  1 followed by numbers that are not squares.
 * https://oeis.org/A164514
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, less, easy
 * Formula:   a(n) = n-1 + floor(1/2 + sqrt(n-1))
 * Range:     u8 n <= 241, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Integer form used below: a(n) = (n-1) + floor( sqrt( (n-1) + floor_sqrt(n-1) ) ) */

/* Integer sqrt for 128‑bit values, returning the floor of the square root.
   It works for any n that fits in unsigned __int128. */
static inline idx_t isqrt_u128(unsigned __int128 n)
{
    unsigned __int128 low  = 0;
    unsigned __int128 high = ((unsigned __int128)1 << 64);  /* 2^64  > sqrt(2^128-1) */

    while (low <= high) {
        unsigned __int128 mid = (low + high) >> 1;
        unsigned __int128 sq  = mid * mid;

        if (sq == n)
            return (idx_t)mid;

        if (sq < n) {
            if ((mid + 1) * (mid + 1) > n)
                return (idx_t)mid;
            low = mid + 1;
        } else {
            if (mid == 0)
                break;
            high = mid - 1;
        }
    }
    return (idx_t)high;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n <= 1)
        return (seq_t)1;

    /* m = n-1 */
    idx_t m = n - 1;

    /* s1 = floor(sqrt(m)) */
    idx_t s1 = isqrt_u128((unsigned __int128)m);

    /* y = m + s1 (might overflow 64‑bit, so use 128‑bit) */
    unsigned __int128 y = (unsigned __int128)m + (unsigned __int128)s1;

    /* s2 = floor(sqrt(y)) */
    idx_t s2 = isqrt_u128(y);

    return (seq_t)(m + s2);
}
