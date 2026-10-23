/*
 * A190250  Positions of 1 in A190248.
 * https://oeis.org/A190250
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the n-th positive integer i such that * 1/4 < {i*phi} <
 *            3/4, where phi = (1+sqrt(5))/2. * This implementation uses only
 *            integer arithmetic. * It computes {i*phi} without floating point
 *            by using the identity * floor(i*phi) = floor((i +
 *            floor(i*sqrt5))/2) * and the fact that i*sqrt5 = floor(...
 * Range:     u8 n <= 128, u16 n <= 5500, u32 n <= 5500, u64 n <= 5500,
 *            u128 n <= 5500, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

static inline idx_t
isqrt_u128(unsigned __int128 val)
{
    uint64_t lo = 0, hi = UINT64_MAX;
    while (lo <= hi) {
        uint64_t mid = lo + ((hi - lo) >> 1);
        unsigned __int128 m2 = (unsigned __int128)mid * mid;
        if (m2 <= val) {
            lo = mid + 1;
        } else {
            if (mid == 0) break;
            hi = mid - 1;
        }
    }
    return lo - 1;
}

seq_t a(idx_t n)
{
    idx_t count = 0;
    idx_t candidate = 1;

    while (count < n) {
        /* a = floor(candidate * sqrt(5)) = floor(sqrt(5 * candidate^2)) */
        unsigned __int128 prod = (unsigned __int128)5 * candidate * candidate;
        idx_t a = isqrt_u128(prod);

        bool even = ((candidate + a) & 1ULL) == 0;

        unsigned __int128 left  = (unsigned __int128)20 * candidate * candidate;
        unsigned __int128 rhs   = (unsigned __int128)4 * a * a
                                + (unsigned __int128)4 * a + 1;

        bool ok;
        if (even)
            ok = left > rhs;
        else
            ok = left < rhs;

        if (ok) {
            count++;
            if (count == n)
                return (seq_t)candidate;
        }
        candidate++;
    }

    /* Unreachable for n >= 1, but keep compiler happy */
    return 0;
}
