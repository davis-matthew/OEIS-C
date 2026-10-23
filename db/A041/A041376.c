/*
 * A041376  Numerators of continued fraction convergents to sqrt(203).
 * https://oeis.org/A041376
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 8, u64 n <= 17, u128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A041376: Numerators of continued fraction convergents to sqrt(203).
   Recurrence: a(0)=14, a(1)=57,
               a(k)=a_k*a(k-1)+a(k-2) for k≥2,
               where a_k = 4 if k is odd, 28 if k is even. */

#include <stdint.h>  /* for idx_t definition if needed */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)14;
    if (n == 1) return (seq_t)57;

    seq_t p0 = (seq_t)14;   /* a(0) */
    seq_t p1 = (seq_t)57;   /* a(1) */
    seq_t p;

    for (idx_t k = 2; k <= n; ++k) {
        seq_t coeff = (k & 1) ? (seq_t)4 : (seq_t)28; /* a_k */
        p = coeff * p1 + p0;
        p0 = p1;
        p1 = p;
    }
    return p1;
}
