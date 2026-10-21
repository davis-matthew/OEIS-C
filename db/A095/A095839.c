/*
 * A095839  a(n) = ((2*n)!/(n!*2^(n-1)))*Integral_{x=1/2..1}
 *          (sqrt(1-x^2)/x)^(2*n) dx.
 * https://oeis.org/A095839
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (4*n-9)*a(n-1) + 6*(n-1)*(2*n-3)*a(n-2), a(0)=1, a(1)=1
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 8, u64 n <= 13, u128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)1;

    seq_t a0 = 1; /* a(n-2) */
    seq_t a1 = 1; /* a(n-1) */

    for (idx_t i = 2; i <= n; ++i)
    {
        long long c1 = 4LL * (long long)i - 9;                 /* 4*i-9 */
        long long c2 = 6LL * (long long)(i - 1) * (2LL * (long long)i - 3); /* 6*(i-1)*(2*i-3) */

        seq_t term = ((seq_t)c2) * a0;               /* c2*a(n-2) */
        if (c1 >= 0)
            term = term + ((seq_t)c1) * a1;           /* + c1*a(n-1) */
        else
            term = term - ((seq_t)(-c1)) * a1;        /* - (-c1)*a(n-1) */

        a0 = a1;
        a1 = term;
    }

    return a1;
}
