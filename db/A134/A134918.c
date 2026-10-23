/*
 * A134918  Ceiling(n^(5/3)).
 * https://oeis.org/A134918
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ceil(n^(5/3))
 * Range:     u8 n <= 27, u16 n <= 776, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Compute n^5 using 128‑bit intermediate to avoid overflow. */
    unsigned __int128 nn = (unsigned __int128)n;
    unsigned __int128 n5 = nn * nn * nn * nn * nn;

    /* Binary search for the integer cube root of n5. */
    unsigned __int128 lo = 0, hi = 1;
    while (hi * hi * hi <= n5) hi <<= 1;          /* upper bound */

    while (hi - lo > 1) {
        unsigned __int128 mid = (lo + hi) >> 1;
        if (mid * mid * mid <= n5)
            lo = mid;
        else
            hi = mid;
    }

    unsigned __int128 res = lo;
    if (res * res * res < n5) ++res;  /* ceil adjustment */

    return (seq_t)res;
}
