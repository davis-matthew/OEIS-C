/*
 * A064873  First of four sequences representing the lexicographical minimal
 *          decomposition of n in 4 squares: n = a(n)^2 + A064874(n)^2 +
 *          A064875(n)^2 + A064876(n)^2.
 * https://oeis.org/A064873
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = minimal k such that n - k^2 is expressible as a sum of
 *            three squares (Legendre's theorem: a number is NOT expressible
 *            as 3 squares iff it is of the form 4^a(8b+7)). Hence we return
 *            the smallest k with n-k^2 NOT of that form
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   forbidden_three_sq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/forbidden_three_sq.h"

/* The required sequence -->
 * a(n) = minimal k with n - k^2 representable as sum of three squares
 */
seq_t a(idx_t n)
{
    for (idx_t k = 0; ; ++k)
    {
        unsigned __int128 sq = (unsigned __int128)k * k;   /* avoid overflow */
        if (sq > n) break;                                /* k too large */
        idx_t rem = n - (idx_t)sq;
        if (!forbidden_three_sq(rem))
            return (seq_t)k;
    }
    /* In practice this point is never reached because every n has a representation
       with some k <= sqrt(n). Return 0 as safe fallback.                */
    return (seq_t)0;
}
