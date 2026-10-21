/*
 * A063016  a(n) is the product of Catalan(n) and (2^(n+1) - 1).
 * https://oeis.org/A063016
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Catalan(n)*(2^(n+1)-1), with offset 0
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 23, u128 n <= 45,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) is the product of the nth Catalan number and (2^(n+1)-1).  The
   Catalan numbers are computed by the recurrence
            C(0) = 1
            C(n) = C(n-1)*(4*n-2)/(n+1)   for n>=1.
   The power of two is built by successive doublings.  All operations
   are performed in seq_t, which may be a 8/16/32/64-bit unsigned type
   or an arbitrary-precision integer type. */

#include <stdint.h>   /* for uint64_t if needed */

seq_t a(idx_t n)
{
    seq_t catalan = 1;                    /* C(0)                      */
    seq_t pow2   = 1;                      /* 2^0                       */
    idx_t i;

    /* compute 2^(n+1) by repeated doubling */
    for (i = 0; i < n + 1; ++i)
        pow2 = pow2 * 2;

    /* compute Catalan(n) by the multiplicative recurrence */
    for (i = 1; i <= n; ++i)
    {
        seq_t mult = (seq_t)(4 * i - 2);          /* 4*i-2 fits in idx_t  */
        catalan = (catalan * mult) / (i + 1);    /* division is exact  */
    }

    return catalan * (pow2 - 1);
}
