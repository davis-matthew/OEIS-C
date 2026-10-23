/*
 * A165441  Table T(k,n) read by antidiagonals: denominator of 1/min(n,k)^2
 *          -1/max(n,k)^2.
 * https://oeis.org/A165441
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl, frac, look, easy
 * Formula:   a(n) = denominator of (1/min(k,n))^2 - (1/max(k,n))^2 read along
 *            the antidiagonals of the infinite table T(k,n). The index n is
 *            interpreted as the (n‑th) entry obtained by traversing the
 *            entries (i,j) with i+j constant and listing (i=1..d) for each
 *            diagonal d
 * Range:     u8 n <= 24, u16 n <= 508, u32 n <= 1035, u64 n <= 1035,
 *            u128 n <= 1035, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    /* n >= 1 (offset 1) */
    if (n == 0) return 0;

    /* Find diagonal d such that d*(d+1)/2 >= n */
    idx_t lo = 0, hi = 1;
    while (((unsigned __int128)hi * (hi + 1)) / 2 < n)
        hi <<= 1;                /* exponential search for upper bound */

    while (lo < hi) {
        idx_t mid = lo + (hi - lo) / 2;
        if (((unsigned __int128)mid * (mid + 1)) / 2 >= n)
            hi = mid;
        else
            lo = mid + 1;
    }
    idx_t d = hi;                           /* diagonal number */

    idx_t prev_sum = ((unsigned __int128)(d - 1) * d) / 2;
    idx_t offset  = n - prev_sum;           /* position in this diagonal, 1‑based */
    idx_t i = offset;                       /* first coordinate (k)   */
    idx_t j = d - offset + 1;               /* second coordinate (n) */

    idx_t m = (i < j) ? i : j;              /* min(i,j) */
    idx_t M = (i > j) ? i : j;              /* max(i,j) */

    /* Compute numerator and denominator using 128‑bit intermediates
       to avoid overflow for n up to at least 10^6. */
    unsigned __int128 mm = m;
    unsigned __int128 MM = M;

    unsigned __int128 num128 = MM * MM - mm * mm;                    /* M^2 - m^2 */
    unsigned __int128 den128 = mm * mm * MM * MM;                    /* m^2 * M^2 */

    seq_t num = (seq_t)num128;
    seq_t den = (seq_t)den128;

    seq_t g = gcd(num, den);                                      /* gcd of numerator and denominator */
    return den / g;                                                /* reduced denominator */
}
