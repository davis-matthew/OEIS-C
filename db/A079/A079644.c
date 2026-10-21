/*
 * A079644  a(n) = (n mod sqrtint(n)).
 * https://oeis.org/A079644
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n mod floor(sqrt(n))
 * Range:     u8 n <= 314, u16 n <= 1023, u32 n <= 1023, u64 n <= 1023,
 *            u128 n <= 1023, big any n
 * Helpers:   isqrt_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/isqrt_u64.h"

seq_t a(idx_t n)
{
    /* n >= 1 guarantees floor(sqrt(n)) >= 1, so no division by zero */
    idx_t r = isqrt_u64(n);          /* floor(sqrt(n)) as an integer */
    seq_t rn = (seq_t)n;              /* cast n to the result type */
    seq_t sr = (seq_t)r;              /* cast sqrt to the result type   */
    return rn % sr;                   /* compute the modulus */
}
