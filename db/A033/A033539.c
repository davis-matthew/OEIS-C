/*
 * A033539  a(0)=1, a(1)=1, a(2)=1, a(n) = 2*a(n-1) + a(n-2) + 1.
 * https://oeis.org/A033539
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(n) = 2*a(n-1) + a(n-2) + 1, a(0)=1, a(1)=1, a(2)=1
 * Range:     u8 n <= 7, u16 n <= 13, u32 n <= 26, u64 n <= 51, u128 n <= 101,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes A033539 by iteration; works for both built‑in integer types and
   arbitrary‑precision seq_t classes. */

#include <stddef.h>

seq_t a(idx_t n)
{
    /* Base cases */
    if (n == 0 || n == 1 || n == 2) {
        return (seq_t)1;
    }

    /* Iterate up to n using the recurrence: a(n) = 2*a(n-1) + a(n-2) + 1 */
    seq_t a0 = (seq_t)1;   /* a(0) */
    seq_t a1 = (seq_t)1;   /* a(1) */
    seq_t a2 = (seq_t)1;   /* a(2) */
    seq_t next;
    idx_t i;

    for (i = 3; i <= n; ++i) {
        next = a2 * (seq_t)2 + a1 + (seq_t)1;
        a0 = a1;
        a1 = a2;
        a2 = next;
    }

    return a2;   /* a(n) */
}
