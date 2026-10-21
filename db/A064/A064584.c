/*
 * A064584  Order of twisted group 2G2(3^(2*n + 1)).
 * https://oeis.org/A064584
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = q^6*(q^2-1)*(q^6+1), where q^2 = 3^(2*n+1)
 * Range:     u8 none, u16 n <= 0, u32 n <= 0, u64 n <= 2, u128 n <= 5,
 *            big any n
 * Helpers:   my_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/my_pow.h"

/* Compute the n-th term of OEIS A064584. */
seq_t a(idx_t n)
{
    idx_t e = 2 * n + 1;            /* exponent for q^2  :   q^2 = 3^e  */
    seq_t q2 = my_pow(3, e);        /* q^2 = 3^e                */
    seq_t q6 = q2 * q2;             /* q^6 = q^2 * q^2          */
    q6 *= q2;                       /* q^6 = q^2 * q^2 * q^2    */

    seq_t term = q6 * (q2 - 1);      /* q6 * (q^2 - 1)           */
    term *= (q6 + 1);               /* *(q6 + 1)                 */
    return term;
}
