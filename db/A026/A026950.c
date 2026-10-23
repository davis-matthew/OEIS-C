/*
 * A026950  a(n) = Sum_{k=0..n} (k+1) * T(n,k), with T given by A026374.
 * https://oeis.org/A026950
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n+2) * (7 + 3*(-1)^n) * 5^floor((n-1)/2) / 4 *
 *            Simplified: for even n=2m, a(n) = (m+1)*5^m; for odd n=2m+1,
 *            a(n) = (2m+3)*5^m
 * Range:     u8 n <= 5, u16 n <= 11, u32 n <= 24, u64 n <= 51, u128 n <= 105,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>   /* for size_t, if needed */

static seq_t pow5_u64(idx_t exp)
{
    seq_t res = 1;
    for (idx_t i = 0; i < exp; ++i)
        res *= 5;
    return res;
}

seq_t a(idx_t n)
{
    if ((n & 1) == 0) {                 /* even n: n = 2*m */
        idx_t m = n >> 1;
        seq_t p = pow5_u64(m);
        return p * (((seq_t)m) + 1);
    } else {                             /* odd n: n = 2*m + 1 */
        idx_t m = (n - 1) >> 1;
        seq_t p = pow5_u64(m);
        return p * (((seq_t)(2 * m)) + 3);
    }
}
