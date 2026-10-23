/*
 * A183691  Number of (n+1) X 3 0..2 arrays with every 2 X 2 subblock
 *          nonsingular.
 * https://oeis.org/A183691
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 8*a(n-1) + 91*a(n-2) + 74*a(n-3) - 208*a(n-4) initial
 *            values: a(1) = 314, a(2) = 4594, a(3) = 66658, a(4) = 969978
 * Range:     u8 none, u16 n <= 2, u32 n <= 7, u64 n <= 15, u128 n <= 31,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* handle the first four terms directly */
    if (n == 1) return (seq_t)314;
    if (n == 2) return (seq_t)4594;
    if (n == 3) return (seq_t)66658;
    if (n == 4) return (seq_t)969978;

    /* iterate the linear recurrence */
    seq_t a1 = (seq_t)314;   /* a(n-4)   */
    seq_t a2 = (seq_t)4594;  /* a(n-3)   */
    seq_t a3 = (seq_t)66658; /* a(n-2)   */
    seq_t a4 = (seq_t)969978;/* a(n-1)   */

    for (idx_t i = 5; i <= n; ++i)
    {
        seq_t next = (seq_t)8*a4 + (seq_t)91*a3 + (seq_t)74*a2;
        next -= (seq_t)208*a1;  /* subtraction after addition is safe */

        /* shift the window for the next iteration */
        a1 = a2;
        a2 = a3;
        a3 = a4;
        a4 = next;
    }
    return a4; /* a(n) */
}
