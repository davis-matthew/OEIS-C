/*
 * A068716  a(n) = 1 if x^2 + 1 = n * y^2 has infinitely many solutions in
 *          integers (x,y), otherwise a(n) = 0.
 * https://oeis.org/A068716
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 if the continued‑fraction period of sqrt(n) is odd, * 0
 *            otherwise (i.e. x²+1 = n·y² has infinitely many integer *
 *            solutions iff n is a non‑square and the period is odd)
 * Range:     u8 n <= 63, u16 n <= 105, u32 n <= 105, u64 n <= 105,
 *            u128 n <= 105, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/floor_sqrt.h"   /* provides floor_sqrt(seq_t x) */

seq_t a(idx_t n)
{
    /* Treat n as a 64‑bit unsigned integer; intermediate values use seq_t. */
    seq_t D = (seq_t)n;           /* D = n */

    /* Compute floor(sqrt(D)) from the supplied utility function. */
    seq_t a0 = floor_sqrt(D);

    /* If n is a perfect square, no solutions exist. */
    if (a0 * a0 == D) {
        return (seq_t)0;
    }

    /* Standard algorithm for the period of the continued fraction of sqrt(D). */
    seq_t m = 0;
    seq_t d = 1;
    seq_t a = a0;
    seq_t period = 0;

    /* Iterate until the repeating term 2*a0 is reached. */
    do {
        m = d * a - m;
        d = (D - m * m) / d;
        a = (a0 + m) / d;
        period++;
    } while (a != 2 * a0);

    /* Return 1 if the period is odd, else 0. */
    return (period & 1) ? (seq_t)1 : (seq_t)0;
}
