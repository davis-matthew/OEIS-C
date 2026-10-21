/*
 * A066932  a(n) is the denominator of b(n) where b(n)=1/b(n-1)+1/b(n-2) with
 *          b(1)=1 and b(2)=2.
 * https://oeis.org/A066932
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, frac
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 7, u32 n <= 8, u64 n <= 10, u128 n <= 11,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(1)=1, a(2)=1; a(n) is the denominator of b(n) where
   b(1)=1, b(2)=2 and b(n)=1/b(n-1)+1/b(n-2). */

#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    if (n <= 2)
        return (seq_t)1;

    seq_t pprev = (seq_t)1;  /* numerator of b(1)   */
    seq_t pcur  = (seq_t)2;  /* numerator of b(2)   */
    seq_t qprev = (seq_t)1;  /* denominator of b(1) */
    seq_t qcur  = (seq_t)1;  /* denominator of b(2) */

    for (idx_t i = 3; i <= n; ++i) {
        seq_t num = qcur * pprev + qprev * pcur;  /* numerator before reduction */
        seq_t den = pcur * pprev;                  /* denominator before reduction */
        seq_t g   = gcd(num, den);                 /* reduce the fraction */
        num /= g;
        den /= g;

        /* shift for the next iteration */
        pprev = pcur;  pcur  = num;
        qprev = qcur;  qcur  = den;
    }

    return qcur;  /* denominator of b(n) */
}
