/*
 * A058306  Denominator of H(n), where H(0)=-1/12, H(n) = number of
 *          equivalence classes of positive definite quadratic forms
 *          a*x^2+b*x*y+c*y^2 with discriminant b^2-4ac = -n, counting forms
 *          equivalent to x^2+y^2 (resp. x^2+x*y+y^2) with multiplicity 1/2
 *          (resp. 1/3).
 * https://oeis.org/A058306
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   a(n) = 12 (n=0); a(n)=3 if n=3k²; a(n)=2 if n=4k²; otherwise 1
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Integer square root: returns floor(sqrt(x)) for 0 ≤ x < 2⁶⁴. */
static idx_t isqrt(idx_t x)
{
    if (x < 2) return x;
    idx_t lo = 0, hi = x;
    while (lo <= hi) {
        idx_t mid = lo + ((hi - lo) >> 1);
        if (mid <= x / mid) {
            lo = mid + 1;
        } else {
            hi = mid - 1;
        }
    }
    return hi;
}

seq_t a(idx_t n)
{
    if (n == 0) {
        return (seq_t)12;
    }
    if ((n % 3) == 0) {
        idx_t m = n / 3;
        idx_t r = isqrt(m);
        if (r * r == m) {
            return (seq_t)3;
        }
    }
    if ((n % 4) == 0) {
        idx_t m = n / 4;
        idx_t r = isqrt(m);
        if (r * r == m) {
            return (seq_t)2;
        }
    }
    return (seq_t)1;
}
