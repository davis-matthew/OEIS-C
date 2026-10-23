/*
 * A028094  Expansion of 1/((1-3x)(1-7x)(1-9x)(1-11x)).
 * https://oeis.org/A028094
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 8, u64 n <= 16, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0)=1, a(1)=30, a(2)=580, a(3)=9210
   a(n) = 30*a(n-1) - 320*a(n-2) + 1410*a(n-3) - 2079*a(n-4) for n >= 4 */
#include <stdint.h>

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)30;
    if (n == 2) return (seq_t)580;
    if (n == 3) return (seq_t)9210;

    seq_t a0 = (seq_t)1,   /* a(n-4)      */
         a1 = (seq_t)30,  /* a(n-3)      */
         a2 = (seq_t)580, /* a(n-2)      */
         a3 = (seq_t)9210,/* a(n-1)      */
         an;             /* a(n) for loop */

    idx_t i;
    for (i = 4; i <= n; ++i) {
        an = ((seq_t)30) * a3
           - ((seq_t)320) * a2
           + ((seq_t)1410) * a1
           - ((seq_t)2079) * a0;
        a0 = a1;
        a1 = a2;
        a2 = a3;
        a3 = an;
    }
    return an;
}
