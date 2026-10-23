/*
 * A040544  Continued fraction for sqrt(568).
 * https://oeis.org/A040544
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy, less, mult
 * Formula:   a(0)=23; for n>0, a(n)=1 if n is odd; if n even with 2-adic
 *            exponent e, a(n)=4 if e==1 else 46. This implements OEIS A040544
 *            (Continued fraction for sqrt(568))
 * Range:     u8 n <= 83, u16 n <= 83, u32 n <= 83, u64 n <= 83, u128 n <= 83,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)23;
    if ((n & 1) == 1)
        return (seq_t)1;

    /* n is even. Compute exponent of 2 in n. */
    idx_t t = n;
    unsigned e = 0;
    while ((t & 1ULL) == 0) {
        ++e;
        t >>= 1;
    }

    if (e == 1)
        return (seq_t)4;
    return (seq_t)46;
}
