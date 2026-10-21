/*
 * A077909  Expansion of 1/((1-x)*(1+x+x^2+2*x^3)).
 * https://oeis.org/A077909
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = -a(n-3) + 2*a(n-4) (n ≥ 4), a(0)=1, a(1)=0, a(2)=0,
 *            a(3)=-1
 * Range:     i8 n <= 20, i16 n <= 38, i32 n <= 75, i64 n <= 148,
 *            i128 n <= 295, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)0;
    if (n == 2) return (seq_t)0;
    if (n == 3) return (seq_t)-1;

    seq_t a0 = (seq_t)1;      /* a(n-4)  */
    seq_t a1 = (seq_t)0;      /* a(n-3)  */
    seq_t a2 = (seq_t)0;      /* a(n-2)  */
    seq_t a3 = (seq_t)-1;     /* a(n-1)  */
    seq_t an;

    for (idx_t i = 4; i <= n; ++i) {
        an = -a1 + (a0 * (seq_t)2);  /* a(i) = -a(i-3) + 2*a(i-4) */
        a0 = a1;
        a1 = a2;
        a2 = a3;
        a3 = an;
    }
    return a3;
}
