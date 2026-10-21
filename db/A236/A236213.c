/*
 * A236213  Number of units in the imaginary quadratic field Q(sqrt(-d)),
 *          where d > 0 is the n-th squarefree number.
 * https://oeis.org/A236213
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4 if the *n*‑th squarefree number is 1, 6 if it is 3, and
 *            2 otherwise (the number of units in Q(√−d) where d is the *n*‑th
 *            squarefree)
 * Range:     u8 n <= 105, u16 n <= 105, u32 n <= 105, u64 n <= 105,
 *            u128 n <= 105, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    /* n is 1‑based.  The only squarefree numbers that give more than two units
       are 1 (4 units) and 3 (6 units).  All other squarefree d give 2 units. */

    if (n == 1) return (seq_t)4;
    if (n == 3) return (seq_t)6;
    return (seq_t)2;
}
