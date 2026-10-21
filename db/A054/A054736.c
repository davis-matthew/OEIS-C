/*
 * A054736  Smallest losing position after your opponent has taken k stones in
 *          a variation of "Fibonacci Nim".
 * https://oeis.org/A054736
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 12, u16 n <= 29, u32 n <= 43, u64 n <= 43, u128 n <= 43,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence:
   a(1) = 4,  a(2) = 8,  a(3) = 11,  a(4) = 15,  a(5) = 21
   a(n) = a(n-1) + a(n-4)   for n ≥ 6
   This recurrence follows from the game theory analysis of the
   Fibonacci‑Nim variation with a multiplier of 3.  The implementation
   uses only a circular buffer of size 5, so it works for any seq_t
   type (plain integer or arbitrary‑precision class) and runs in O(n)
   time with O(1) memory. */
#include <stddef.h>

seq_t a(idx_t n)
{
    /* Handle undefined or zero index gracefully */
    if (n < 1) return 0;

    /* Base values a(1)…a(5) */
    static const seq_t init[6] = {0, 4, 8, 11, 15, 21};

    if (n <= 5) return init[n];

    /* Circular buffer to store the last five terms:
       buf[i % 5] holds a(i) when the loop has processed up to i. */
    seq_t buf[5];

    /* Initialise buffer with a(1)…a(5) */
    for (idx_t i = 1; i <= 5; ++i)
        buf[i % 5] = init[i];

    /* Compute up to a(n) using the recurrence */
    for (idx_t i = 6; i <= n; ++i)
    {
        seq_t ai = buf[(i - 1) % 5] + buf[(i - 4) % 5];
        buf[i % 5] = ai;
    }

    return buf[n % 5];
}
