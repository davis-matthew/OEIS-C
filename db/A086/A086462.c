/*
 * A086462  Expansion of (1+x)*(1+4*x)/((1-x)*(1-4*x)).
 * https://oeis.org/A086462
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 10*(4^n-1)/3 + 0^n (with 0^0 = 1)
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 31, u128 n <= 63,
 *            big any n
 * Helpers:   pow4
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow4.h"

seq_t a(idx_t n)
{
    if (n == 0u) {
        /* 0^0 is taken as 1 to give a(0) = 1 */
        return (seq_t)1;
    }
    /* Compute 4^n first */
    seq_t v = pow4(n);          /* v = 4^n               */
    v -= (seq_t)1;              /* v = 4^n - 1           */
    v /= (seq_t)3;              /* v = (4^n - 1) / 3     */
    v *= (seq_t)10;             /* v = 10*(4^n - 1) / 3  */
    return v;                   /* a(n) for n > 0        */
}
