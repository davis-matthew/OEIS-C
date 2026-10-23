/*
 * A122882  Array of T(n,m)=1*5*...*(4n-3)*3*7*...*(4m-1)*2^(n+m)/(n+m)! by
 *          antidiagonals.
 * https://oeis.org/A122882
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   a(n) = T( i(n), j(n) ) where i(n) = s - (n - s*(s+1)/2) j(n) =
 *            (n - s*(s+1)/2) and T(n,m) = 1*5*...*(4n-3) * 3*7*...*(4m-1) *
 *            2^(n+m) / (n+m)!
 * Range:     u8 n <= 8, u16 n <= 26, u32 n <= 42, u64 n <= 42, u128 n <= 42,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence for T:
   T(0,0)=1
   T(n,m) = T(n-1,m)*(8*n-6)/(n+m)   if n>0
   T(n,m) = T(n,m-1)*(8*m-2)/(n+m)   if m>0 */

#include <math.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* Determine the di‑agonal s such that s(s+1)/2 <= n < (s+1)(s+2)/2 */
    uint64_t idx = n;                     /* safe conversion for sqrt */
    uint64_t s = (uint64_t)((sqrt((long double)(8*idx + 1)) - 1) / 2);
    uint64_t start = s * (s + 1) / 2;
    uint64_t pos   = idx - start;         /* 0 … s */
    uint64_t ni = s - pos;                /* n coordinate */
    uint64_t mj = pos;                    /* m coordinate */

    /* Compute T(ni, mj) by moving first along n, then along m */
    seq_t t = 1;

    for (uint64_t a = 0; a < ni; ++a) {
        seq_t num = (seq_t)(8 * (a + 1) - 6);
        seq_t den = (seq_t)(a + 1);
        t = t * num / den;
    }

    for (uint64_t b = 0; b < mj; ++b) {
        seq_t num = (seq_t)(8 * (b + 1) - 2);
        seq_t den = (seq_t)(ni + b + 1);
        t = t * num / den;
    }

    return t;
}
