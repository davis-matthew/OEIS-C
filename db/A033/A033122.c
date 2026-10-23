/*
 * A033122  Base-4 digits are, in order, the first n terms of the periodic
 *          sequence with initial period 1,0,1.
 * https://oeis.org/A033122
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(1)=1, a(2)=4, a(3)=17, a(4)=69, a(n) = 4*a(n-1) + a(n-3) -
 *            4*a(n-4) (n ≥ 5)
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 16, u64 n <= 32, u128 n <= 64,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;

    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)4;
    if (n == 3) return (seq_t)17;
    if (n == 4) return (seq_t)69;

    seq_t a1 = (seq_t)1;   /* a(n-4)   */
    seq_t a2 = (seq_t)4;   /* a(n-3)   */
    seq_t a3 = (seq_t)17;  /* a(n-2)   */
    seq_t a4 = (seq_t)69;  /* a(n-1)   */
    seq_t next;

    for (idx_t i = 5; i <= n; ++i) {
        next = (seq_t)4 * a4 + a2 - (seq_t)4 * a1;
        a1 = a2;
        a2 = a3;
        a3 = a4;
        a4 = next;
    }
    return a4;
}
