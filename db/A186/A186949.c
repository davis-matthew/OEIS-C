/*
 * A186949  a(n) = 2^n - 2*(binomial(1,n) - binomial(0,n)).
 * https://oeis.org/A186949
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2^n - 2*(binomial(1,n) - binomial(0,n))
 * Range:     u8 n <= 7, u16 n <= 15, u32 n <= 31, u64 n <= 63, u128 n <= 127,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Equivalently: a(0)=1, a(1)=0, and for n≥2, a(n)=2^n. */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    if (n == (idx_t)1)
        return (seq_t)0;   /* a(1) = 0 */
    return pow2(n);         /* a(0)=1, a(n)=2^n for n≥2 */
}
