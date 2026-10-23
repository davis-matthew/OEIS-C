/*
 * A183630  Number of (n+1) X 8 0..2 arrays with every 2 X 2 subblock summing
 *          to 4.
 * https://oeis.org/A183630
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 6050 + 127*2^(n+2) + 3^(n+1) (OEIS A183630)
 * Range:     u8 none, u16 n <= 6, u32 n <= 19, u64 n <= 39, u128 n <= 79,
 *            big any n
 * Helpers:   pow2, pow3
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"   /* seq_t pow2(idx_t exp);   computes 2^exp  */
#include "../utils/pow3.h"   /* seq_t pow3(idx_t exp);   computes 3^exp  */

seq_t a(idx_t n)
{
    /* n is guaranteed to be >= 1 by the problem definition */
    seq_t result = (seq_t)6050;            /* add the constant term 6050   */
    seq_t t = pow2(n + 2);                 /* compute 2^(n+2)              */
    t = t * (seq_t)127;                    /* multiply by 127               */
    result += t;                           /* add the 127*2^(n+2) term     */
    result += pow3(n + 1);                 /* add the 3^(n+1) term         */
    return result;
}
