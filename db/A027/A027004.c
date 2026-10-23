/*
 * A027004  a(n) = T(2*n+1,n+1), T given by A026998.
 * https://oeis.org/A027004
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 10, u32 n <= 21, u64 n <= 44, u128 n <= 90,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: A027004
   Recurrence: a(0)=1, a(1)=8, a(2)=26
                a(n) = 4*a(n-1) - 4*a(n-2) + a(n-3)   for n>2
*/
#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0U) return (seq_t)1;
    if (n == 1U) return (seq_t)8;
    if (n == 2U) return (seq_t)26;

    seq_t a0 = (seq_t)1;   /* a(0) */
    seq_t a1 = (seq_t)8;   /* a(1) */
    seq_t a2 = (seq_t)26;  /* a(2) */
    seq_t an;

    for (idx_t i = 3; i <= n; ++i) {
        an = (seq_t)4 * a2 - (seq_t)4 * a1 + a0;
        a0 = a1;
        a1 = a2;
        a2 = an;
    }
    return a2;
}
