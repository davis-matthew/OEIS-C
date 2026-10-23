/*
 * A118829  Denominators of the convergents of the 2-adic continued fraction
 *          of zero given by A118827.
 * https://oeis.org/A118829
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  frac, sign
 * Formula:   a(4*n) = -(-1)^n * 2 * A080277(n) a(4*n+1) = -(-1)^n * (2 *
 *            A080277(n) – 1) a(4*n+2) = (-1)^n * (2 * A080277(n) – 2)
 *            a(4*n-1) = (-1)^n A080277 satisfies A080277(0)=0 and for n>0:
 *            A080277(n)=n+2*A080277(n/2)
 * Range:     i8 n <= 63, i16 n <= 77, i32 n <= 77, i64 n <= 77, i128 n <= 77,
 *            big any n
 * Helpers:   A080277
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/A080277.h"

seq_t a(idx_t n)
{
    /*  n ≥ 1  –  use residues modulo 4  */
    switch (n & 3)
    {
    case 0:   /* a(4*t)   */
    {
        idx_t t     = n >> 2;
        seq_t val   = A080277(t);
        seq_t tmp   = (seq_t)2 * val;
        return (t & 1) ? tmp : -tmp;
    }
    case 1:   /* a(4*t+1) */
    {
        idx_t t     = (n - 1) >> 2;
        seq_t val   = A080277(t);
        seq_t sub   = (seq_t)2 * val - 1;
        return (t & 1) ? sub : -sub;
    }
    case 2:   /* a(4*t+2) */
    {
        idx_t t     = (n - 2) >> 2;
        seq_t val   = A080277(t);
        seq_t sub   = (seq_t)2 * val - 2;
        return (t & 1) ? -sub : sub;
    }
    case 3:   /* a(4*t-1) */
    {
        idx_t k = (n + 1) >> 2;   /* k = (n+1)/4  */
        return (k & 1) ? -1 : 1;
    }
    }
    return 0;   /* unreachable for n ≥ 1 */
}
