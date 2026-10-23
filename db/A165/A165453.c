/*
 * A165453  Linear interpolation of the sequence that maps an entry of A002378
 *          to the corresponding entry of A006331.
 * https://oeis.org/A165453
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = (1/3)*t*(3*n + 1 - t^2), where t = floor(sqrt(n)+1/2).
 *            This equals Sum_{k=1..n} floor(sqrt(k)+1/2)
 * Range:     u8 n <= 52, u16 n <= 59, u32 n <= 59, u64 n <= 59, u128 n <= 59,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence offset = 0. */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

/* Integer square root: floor(sqrt(x)) for unsigned 64-bit values. */
static inline idx_t isqrt(idx_t x)
{
    idx_t lo = 0, hi = x;
    while (lo < hi) {
        idx_t mid = lo + ((hi - lo + 1) >> 1);
        if (mid > 0 && mid > x / mid) /* mid*mid > x */
            hi = mid - 1;
        else
            lo = mid;
    }
    return lo;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;

    /* Compute floor(sqrt(n)). */
    idx_t s = isqrt(n);
    seq_t ss = (seq_t)s;

    /* Determine t = round(sqrt(n)). */
    idx_t diff = n - s * s;
    seq_t t = ss;
    if (diff > s)
        t = ss + 1;

    seq_t ns = (seq_t)n;
    seq_t part = (seq_t)3 * ns + 1 - t * t;
    seq_t result = t * part / 3;
    return result;
}
