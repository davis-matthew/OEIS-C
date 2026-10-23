/*
 * A152107  a(n) = ((6+sqrt(5))^n+(6-sqrt(5))^n)/2.
 * https://oeis.org/A152107
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 21, u128 n <= 42,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = 12*a(n-1) - 31*a(n-2),  a(0)=1, a(1)=6 */

#include <stddef.h>

seq_t a(idx_t n)
{
    /* Handle the first two terms directly. */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)6;

    seq_t a0 = (seq_t)1; /* a(0) */
    seq_t a1 = (seq_t)6; /* a(1) */
    seq_t an;

    /* Iterate the linear recurrence up to the desired index. */
    for (idx_t i = 2; i <= n; ++i) {
        an = (seq_t)12 * a1 - (seq_t)31 * a0;
        a0 = a1;
        a1 = an;
    }

    return a1;
}
