/*
 * A027992  a(n) = 1*T(n,0) + 2*T(n,1) + ... + (2n+1)*T(n,2n), T given by
 *          A027926.
 * https://oeis.org/A027992
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2^n*(3n-1)+2
 * Range:     u8 n <= 4, u16 n <= 10, u32 n <= 25, u64 n <= 27, u128 n <= 27,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;
    seq_t power = pow2(n);                 /* compute 2^n */
    seq_t mult  = (seq_t)n * (seq_t)3 - (seq_t)1;  /* compute (3n-1) */
    power *= mult;                          /* multiply 2^n * (3n-1) */
    power += (seq_t)2;                       /* add 2 */
    return power;
}
