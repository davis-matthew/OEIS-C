/*
 * A212704  a(n) = 9*n*10^(n-1).
 * https://oeis.org/A212704
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 9*n*10^(n-1)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 18, u128 n <= 37,
 *            big any n
 * Helpers:   pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow10.h"

seq_t a(idx_t n)
{
    /* n >= 1 by definition of the sequence */
    seq_t pow = pow10(n - 1);                     /* compute 10^(n-1)   */
    seq_t mul = (seq_t)9 * (seq_t)n;              /* compute 9*n         */
    return pow * mul;                            /* final result        */
}
