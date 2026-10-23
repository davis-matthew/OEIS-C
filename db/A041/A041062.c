/*
 * A041062  Numerators of continued fraction convergents to sqrt(38).
 * https://oeis.org/A041062
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 9, u64 n <= 19, u128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0)=6, a(1)=37, 
   for n >= 2, a(n) = ((n%2==0)?12:6) * a(n-1) + a(n-2) */
#include <stddef.h>

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)6;
    if (n == 1) return (seq_t)37;

    seq_t p_nm2 = (seq_t)6;   // a(n-2)
    seq_t p_nm1 = (seq_t)37;  // a(n-1)
    seq_t p_n;

    for (idx_t i = 2; i <= n; ++i) {
        /* coefficient alternates 12,6,12,6,... */
        unsigned int coeff = (i & 1) ? 6u : 12u;  /* i odd → 6, i even → 12 */
        p_n = (seq_t)coeff * p_nm1 + p_nm2;        /* a(i) = coeff*a(i-1)+a(i-2) */
        p_nm2 = p_nm1;
        p_nm1 = p_n;
    }

    return p_n;
}
