/*
 * A062862  Number of ways n can be written as the sum of squares of
 *          consecutive numbers (possibly including squares of negative
 *          numbers).
 * https://oeis.org/A062862
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of representations of n as a sum of consecutive
 *            squares (the starting integer may be negative). The count is the
 *            number of integer pairs (m,k) with k≥1,m∈ℤ such that n = k*m^2 +
 *            m*k(k-1) + k(k-1)(2k-1)/6. This reduces to the quadratic
 *            condition k*(12n – k(k²–1)) must be divisible by...
 * Range:     u8 n <= 104, u16 n <= 104, u32 n <= 104, u64 n <= 104,
 *            u128 n <= 104, big any n
 * Helpers:   is_square128
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/is_square128.h"

seq_t a(idx_t n)
{
    seq_t count = 0;
    for (idx_t k = 1; ; ++k) {
        /* left = 12*n, right = k*(k^2 – 1)  (all 128‑bit to avoid overflow) */
        __int128 left  = ( __int128)12 * ( __int128)n;
        __int128 kk    = ( __int128)k * ( __int128)k;   /* k^2          */
        __int128 right = ( __int128)k * (kk - 1);       /* k(k^2–1)     */

        if (left < right)    /* further k will only increase right */
            break;

        __int128 numerator = left - right;             /* >= 0          */
        __int128 denom     = (__int128)3 * ( __int128)k;/* > 0          */

        if (numerator % denom != 0)
            continue;                                 /* R not integer */

        __int128 R = numerator / denom;                /* integer          */

        __int128 s;
        if (!is_square128(R, &s))
            continue;                                 /* R not a square   */

        if (s == 0) {
            /* s = 0 gives a single solution if L (k) is odd. */
            if (((k - 1) & 1) == 0)
                count += 1;
        } else {
            /* two solutions (±s) if s and k-1 have same parity. */
            if (((s & 1) == ((k - 1) & 1)))
                count += 2;
        }
    }
    return count;
}
