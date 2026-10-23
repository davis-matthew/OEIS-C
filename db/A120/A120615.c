/*
 * A120615  a(n) = Sum_{k=0..n} floor(phi*floor(k/phi)) where phi =
 *          (1+sqrt(5))/2.
 * https://oeis.org/A120615
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n*(n-3)/2 + ceil(n/phi) (phi = (1+sqrt5)/2)
 * Range:     u8 n <= 23, u16 n <= 54, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* For n==0 the offset is 1, but we return 0 for safety */
    if (n == 0)
        return 0;

    /* Binary search for m = ceil(n/phi) using exact inequality
       m*phi >= n  <=> 5*m^2 >= (2*n - m)^2   because phi = (1+√5)/2  */
    idx_t low = 0, high = n;
    while (low < high) {
        idx_t mid = (low + high) >> 1;
        unsigned __int128 lhs = (unsigned __int128)5 * mid * mid;
        unsigned __int128 rhs = (unsigned __int128)(2 * n - mid) *
                                (2 * n - mid);
        if (lhs >= rhs)
            high = mid;
        else
            low = mid + 1;
    }
    idx_t m = low; /* ceil(n/phi) */

    /* Compute a(n) = n*(n-3)/2 + m, using signed arithmetic for the
       potentially negative intermediate when n is small. */
    long long nn = (long long)n;
    long long val = (nn * (nn - 3)) / 2 + (long long)m;

    return (seq_t)val;
}
