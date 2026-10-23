/*
 * A020552  Sixth Fibonacci polynomial evaluated at x=n!.
 * https://oeis.org/A020552
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, changed
 * Formula:   a(n) = (n!)^5 + 4*(n!)^3 + 3*(n!)
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 4, u64 n <= 7, u128 n <= 11,
 *            big any n
 * Helpers:   factorial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/factorial.h"

/* Compute a(n) = (n!)^5 + 4*(n!)^3 + 3*(n!) for any n >= 0.
   Uses seq_t operations which are defined for all supported integer sizes
   (fixed‑size or arbitrary‑precision). */
seq_t a(idx_t n)
{
    /* factorial of n */
    seq_t f = factorial(n);

    /* compute f^2, f^3, f^5 by successive multiplication */
    seq_t f2 = f * f;          /* f^2 */
    seq_t f3 = f2 * f;         /* f^3 */
    seq_t f5 = f3 * f2;        /* f^5 = f^3 * f^2 */

    /* a(n) = f5 + 4*f3 + 3*f */
    seq_t result = f5 + (f3 * 4) + (f * 3);
    return result;
}
