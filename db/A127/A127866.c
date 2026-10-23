/*
 * A127866  Number of L-shaped tiles in all tilings of a 2 X n board with 1 X
 *          1 and L-shaped tiles (where the L-shaped tiles cover 3 squares).
 * https://oeis.org/A127866
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 20, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A127866 – Number of L‑shaped tiles in all tilings of a 2×n board
 * with 1×1 and L‑shaped tiles.  The sequence satisfies the linear
 * recurrence
 *
 *     a(2)=4,  a(3)=12,  a(4)=52,
 *     a(n)=3·a(n-1)+4·a(n-2)−8·a(n-3)−12·a(n-4)−4·a(n-5)   for n≥5
 *
 * (The recurrence follows from the rational generating function
 *  4x²/((1+x)(1−2x−2x²)²).) */

#include <stddef.h>

seq_t a(idx_t n)
{
    /* Return 0 for n<2: the sequence is defined only for n≥2 */
    if (n < 2) return 0;

    if (n == 2) return (seq_t)4;
    if (n == 3) return (seq_t)12;
    if (n == 4) return (seq_t)52;

    /* Initial values for the recurrence */
    seq_t a0 = 0;     /* a(0) */
    seq_t a1 = 0;     /* a(1) */
    seq_t a2 = 4;     /* a(2) */
    seq_t a3 = 12;    /* a(3) */
    seq_t a4 = 52;    /* a(4) */
    seq_t cur;

    /* Compute up to a(n) using the recurrence */
    for (idx_t i = 5; i <= n; ++i) {
        cur = 3 * a4 + 4 * a3 - 8 * a2 - 12 * a1 - 4 * a0;

        /* Shift the window of previous terms */
        a0 = a1;
        a1 = a2;
        a2 = a3;
        a3 = a4;
        a4 = cur;
    }

    return a4;   /* a(n) */
}
