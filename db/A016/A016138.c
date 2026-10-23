/*
 * A016138  Expansion of 1/((1-3*x)*(1-7*x)).
 * https://oeis.org/A016138
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (7^(n+1)-3^(n+1))/4 Recurrence: a(0)=1, a(1)=10,
 *            a(n)=10*a(n-1)-21*a(n-2)
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 11, u64 n <= 22, u128 n <= 45,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>  /* for fixed-width types, optional */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)10;

    seq_t a0 = (seq_t)1;          /* a(0) */
    seq_t a1 = (seq_t)10;         /* a(1) */
    seq_t an;

    for (idx_t i = 2; i <= n; ++i) {
        seq_t tmp = a1 * (seq_t)10;                     /* 10 * a(n-1) */
        tmp -= a0 * (seq_t)21;                         /* - 21 * a(n-2) */
        an = tmp;
        a0 = a1;
        a1 = an;
    }
    return a1;
}
