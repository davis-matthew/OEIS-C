/*
 * A239680  Number of pairs (x,y) in {0,...,n-1}^2 such that x^2+y^2 divides
 *          n.
 * https://oeis.org/A239680
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = # {(x,y) in [0,n-1]^2 : n mod (x^2 + y^2) == 0}
 * Range:     u8 n <= 83, u16 n <= 83, u32 n <= 83, u64 n <= 83, u128 n <= 83,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <math.h>

/* Integer square‑root of an unsigned 64‑bit integer.
   Results are computed as floor(sqrt(n)) and are guaranteed to be correct. */
static inline idx_t isqrt_u64(idx_t n)
{
    idx_t s = (idx_t) sqrt((long double) n);      /* rough estimate */
    /* adjust downwards if necessary */
    while ((idx_t)((unsigned __int128)s * s) > n) --s;
    /* adjust upwards if necessary */
    while ((unsigned __int128)(s + 1) * (s + 1) <= n) ++s;
    return s;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;                /* undefined for n = 0, harmless */

    seq_t count = 0;

    /* Enumerate all positive divisors d of n. */
    for (idx_t d = 1; d <= n / d; ++d) {
        if (n % d == 0) {
            /* ---- process divisor d ---- */
            idx_t limit = isqrt_u64(d);
            for (idx_t x = 0; x <= limit; ++x) {
                idx_t x2   = x * x;
                idx_t rem  = d - x2;
                idx_t y    = isqrt_u64(rem);
                if ((unsigned __int128)y * y == rem) {
                    if (x < n && y < n) count++;
                }
            }

            /* ---- process the paired divisor d2 = n/d, if distinct ---- */
            idx_t d2 = n / d;
            if (d2 != d) {
                idx_t limit2 = isqrt_u64(d2);
                for (idx_t x = 0; x <= limit2; ++x) {
                    idx_t x2   = x * x;
                    idx_t rem  = d2 - x2;
                    idx_t y    = isqrt_u64(rem);
                    if ((unsigned __int128)y * y == rem) {
                        if (x < n && y < n) count++;
                    }
                }
            }
        }
    }

    return count;
}
