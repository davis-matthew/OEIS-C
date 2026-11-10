/*
 * A094956  Diagonal T(n+2,n) of array A094954.
 * https://oeis.org/A094956
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = T(n+2,n), where T(k,1) = 1, T(k,2) = k-1, T(k,m) =
 *            k*T(k,m-1) - T(k,m-2) for m >= 3 (g.f. x(1-x)/(1-kx+x^2))
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 9, u64 n <= 16, u128 n <= 17,
 *            big any n
 * Validated: partial, pass, 17 terms through n = 17 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    seq_t k = (seq_t)n + 2;
    seq_t p = 1;            /* T(k,1) */
    if (n == 1) return p;
    seq_t q = k - 1;        /* T(k,2) */
    for (idx_t i = 3; i <= n; ++i)
    {
        seq_t r = k * q - p;
        p = q;
        q = r;
    }
    return q;
}
