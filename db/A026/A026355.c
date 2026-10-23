/*
 * A026355  a(n) = least k such that s(k) = n+1, where s = A026354.
 * https://oeis.org/A026355
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor((n-1)*phi) + 2, where phi = (1+√5)/2. * The
 *            computation avoids floating‑point arithmetic by using the *
 *            identity * floor(m*phi) = (m + floor(m*√5)) / 2, for m = n-1. *
 *            The integer square root of 5*m*m is obtained with a plain binary
 *            * search, which works for all integer types `seq_t`...
 * Range:     u8 n <= 4, u16 n <= 18, u32 n <= 68, u64 n <= 68, u128 n <= 68,
 *            big any n
 * Helpers:   isqrt_custom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/isqrt_custom.h"

/* --- sequence a(n) implementation ----------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) {
        return (seq_t)1;
    }

    /* m = n-1 */
    seq_t m = (seq_t)(n - 1);

    /* Compute floor(m*sqrt(5)) as floor(sqrt(5*m*m)) */
    seq_t t  = (seq_t)5 * m * m;
    seq_t s  = isqrt_custom(t);           /* floor(m*sqrt5) */

    /* floor(m*phi) = (m + s) / 2  (integer division) */
    seq_t floor_m_phi = (m + s) / 2;

    /* a(n) = floor(m*phi) + 2 */
    return floor_m_phi + (seq_t)2;
}
