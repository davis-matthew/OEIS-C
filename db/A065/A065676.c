/*
 * A065676  The exponent of 2 in the fractions of the whole ]0, inf[
 *          Stern-Brocot tree (A007305/A047679) [1/1, 1/2, 2/1, 1/3, 2/3, 3/2,
 *          3/1, 1/4, 2/5, 3/5, 3/4, 4/3, 5/3, 5/2, 4/1, ...].
 * https://oeis.org/A065676
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = v₂(numerator(n)) – v₂(denominator(n)), where the fraction
 *            numerator(n)/denominator(n) is the nth element of the
 *            Stern‑Brocot tree listed breadth‑first. The fraction is found by
 *            interpreting the binary representation of n (with the leading 1
 *            removed) as a path: 0 = go left, 1 = go right. Th...
 * Range:     i8 n <= 108, i16 n <= 108, i32 n <= 108, i64 n <= 108,
 *            i128 n <= 108, big any n
 * Helpers:   v2_seq_t
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/v2_seq_t.h"

seq_t a(idx_t n)
{
    if (n == 0)           /* safety – the sequence starts at n=1 */
        return 0;

    /* left and right bounds of the Stern‑Brocot interval */
    seq_t lnum = 0, lden = 1;
    seq_t rnum = 1, rden = 0;

    /* number of bits after the leading 1 of n */
    idx_t bits = 0;
    idx_t t = n;
    while (t > 1)
    {
        t >>= 1;
        ++bits;
    }

    /* walk the bits from most significant to least significant */
    for (idx_t i = bits; i > 0; --i)
    {
        idx_t bit = (n >> (i - 1)) & 1;
        seq_t m_num = lnum + rnum;
        seq_t m_den = lden + rden;
        if (bit == 0)
        {
            /* go left: new right bound is the mediant */
            rnum = m_num;
            rden = m_den;
        }
        else
        {
            /* go right: new left bound is the mediant */
            lnum = m_num;
            lden = m_den;
        }
    }

    /* the target fraction is the last mediant computed */
    seq_t p = lnum + rnum;  /* numerator   */
    seq_t q = lden + rden;  /* denominator */

    int e_num = v2_seq_t(p);
    int e_den = v2_seq_t(q);

    return (seq_t)e_num - (seq_t)e_den;
}
