/*
 * A183039  a(n) = A051064(n)*3^A051064(n) where A051064(n) equals the 3-adic
 *          valuation of 3n.
 * https://oeis.org/A183039
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (1+v₃(n)) * 3^(1+v₃(n)) where v₃(n) is the 3-adic
 *            valuation of n
 * Range:     u8 n <= 26, u16 n <= 6560, u32 n <= 9819, u64 n <= 9819,
 *            u128 n <= 9819, big any n
 * Helpers:   pow3
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow3.h"

seq_t a(idx_t n)
{
    if (n == 0) /* undefined for the sequence offset, but guard against division by zero */
        return (seq_t)0;

    idx_t m = n;
    idx_t v = 0;
    while (m % 3 == 0) {
        v++;
        m /= 3;
    }

    idx_t e = v + 1;           /* e = 1 + v₃(n) = v₃(3n) */
    seq_t p = pow3(e);         /* compute 3^e */
    return (seq_t)e * p;       /* e * 3^e */
}
