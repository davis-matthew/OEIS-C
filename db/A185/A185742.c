/*
 * A185742  Number of (n+1) X 2 binary arrays with every 2 X 2 subblock trace
 *          equal to some horizontal or vertical neighbor 2 X 2 subblock
 *          trace.
 * https://oeis.org/A185742
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 10, u32 n <= 21, u64 n <= 42, u128 n <= 86,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence:
   a(n) = 3*a(n-1) - a(n-2) - 2*a(n-3) + 8*a(n-4)
   initial terms: a(1)=0, a(2)=24, a(3)=40, a(4)=144
*/
#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)0;
    if (n == 2) return (seq_t)24;
    if (n == 3) return (seq_t)40;
    if (n == 4) return (seq_t)144;

    seq_t a1 = (seq_t)0;   /* a(n-4) */
    seq_t a2 = (seq_t)24;  /* a(n-3) */
    seq_t a3 = (seq_t)40;  /* a(n-2) */
    seq_t a4 = (seq_t)144; /* a(n-1) */
    seq_t ai;

    for (idx_t i = 5; i <= n; ++i) {
        ai = 3 * a4 - a3 - 2 * a2 + 8 * a1;
        a1 = a2;
        a2 = a3;
        a3 = a4;
        a4 = ai;
    }
    return a4;
}
