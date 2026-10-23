/*
 * A119661  a(n) = floor(Pi/arccos(n/(n+1))).
 * https://oeis.org/A119661
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor(Pi/arccos(n/(n+1))) (offset: 1)
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <math.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* offset rule – not used */

    /* Precompute Pi as a long double */
    static const long double PI = acosl(-1.0L);

    /* t = n/(n+1) as a long double */
    long double t = (long double)n / ((long double)n + 1.0L);

    /* Upper bound for m = a(n). Roughly m ≈ Pi / sqrt(2/(n+1)) */
    idx_t hi = (idx_t)(2.0L * sqrtl((long double)n) + 10.0L);
    if (hi < 1) hi = 1;

    idx_t lo = 1;
    idx_t best = 1;

    while (lo <= hi) {
        idx_t mid = lo + ((hi - lo) >> 1);
        long double cos_val = cosl(PI / (long double)mid);
        if (cos_val <= t) {
            best = mid;            /* cos(pi/m) <= t  →  m is a candidate */
            lo = mid + 1;
        } else {
            hi = mid - 1;
        }
    }

    return (seq_t)best;
}
