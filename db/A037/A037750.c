/*
 * A037750  Decimal expansion of a(n) is given by the first n terms of the
 *          periodic sequence with initial period 2,3,0,1.
 * https://oeis.org/A037750
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 10, u64 n <= 19, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(1)=2, a(2)=23, a(3)=230, a(4)=2301, a(5)=23012
   a(n) = 10*a(n-1) + a(n-4) - 10*a(n-5)  for n >= 6
*/
#include <stdint.h>  /* for integer types, no influence on seq_t */

seq_t a(idx_t n)
{
    /* n >= 1 (offset 1) */
    static const seq_t init[5] = { (seq_t)2, (seq_t)23, (seq_t)230,
                                   (seq_t)2301, (seq_t)23012 };
    if (n <= 5)
        return init[n-1];

    seq_t a1 = init[0]; /* a(n-5) */
    seq_t a2 = init[1]; /* a(n-4) */
    seq_t a3 = init[2]; /* a(n-3) */
    seq_t a4 = init[3]; /* a(n-2) */
    seq_t a5 = init[4]; /* a(n-1) */

    for (idx_t i = 6; i <= n; ++i) {
        seq_t next = ((seq_t)10) * a5 + a2 - ((seq_t)10) * a1;
        a1 = a2;
        a2 = a3;
        a3 = a4;
        a4 = a5;
        a5 = next;
    }

    return a5;
}
