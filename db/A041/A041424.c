/*
 * A041424  Numerators of continued fraction convergents to sqrt(228).
 * https://oeis.org/A041424
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 14, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0) = 15, a(1) = 151,
   a(n) = (10 if n is odd, 30 if n is even)·a(n−1) + a(n−2) for n ≥ 2. */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)15;
    if (n == 1) return (seq_t)151;

    seq_t prevprev = (seq_t)15;   /* a(0) */
    seq_t prev     = (seq_t)151;  /* a(1) */
    seq_t cur;

    for (idx_t k = 2; k <= n; ++k)
    {
        seq_t coeff = (k & 1) ? (seq_t)10 : (seq_t)30;  /* 10 if k odd, 30 if even */
        cur = coeff * prev + prevprev;
        prevprev = prev;
        prev = cur;
    }
    return cur;
}
