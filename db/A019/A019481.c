/*
 * A019481  a(n) = 3*a(n-1) + a(n-2) - 2*a(n-3) (agrees with A019480 for n <=
 *          19 only).
 * https://oeis.org/A019481
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 8, u32 n <= 18, u64 n <= 37, u128 n <= 76,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = 3*a(n-1) + a(n-2) - 2*a(n-3)
   Initial values: a(0) = 4, a(1) = 12, a(2) = 37 */
#include <stdint.h>

seq_t a(idx_t n)
{
    /* Handle small indices directly */
    if (n == 0) return (seq_t)4;
    if (n == 1) return (seq_t)12;
    if (n == 2) return (seq_t)37;

    /* Recurrence computation, only last three values needed */
    seq_t a0 = 4;   /* a(n-3) */
    seq_t a1 = 12;  /* a(n-2) */
    seq_t a2 = 37;  /* a(n-1) */
    seq_t a3 = 0;   /* a(n) */

    for (idx_t i = 3; i <= n; ++i) {
        a3 = 3 * a2 + a1 - 2 * a0;
        a0 = a1;
        a1 = a2;
        a2 = a3;
    }
    return a3;
}
