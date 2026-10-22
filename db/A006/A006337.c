/*
 * A006337  An "eta-sequence": a(n) = floor( (n+1)*sqrt(2) ) - floor(
 *          n*sqrt(2) ).
 * https://oeis.org/A006337
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(n) = floor((n+1)*sqrt(2)) - floor(n*sqrt(2)), with
 *            floor(m*sqrt(2)) = isqrt(2*m^2)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static unsigned __int128 isqrt_u128(unsigned __int128 x)
{
    unsigned __int128 r = 0, bit = (unsigned __int128)1 << 126;
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

static unsigned __int128 floor_m_sqrt2(uint64_t m)
{
    unsigned __int128 mm = (unsigned __int128)m;
    return isqrt_u128(2 * mm * mm);
}

seq_t a(idx_t n)
{
    unsigned __int128 d = floor_m_sqrt2((uint64_t)n + 1) - floor_m_sqrt2((uint64_t)n);
    return (seq_t)(int)d;
}
