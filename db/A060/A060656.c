/*
 * A060656  a(n) = 2*a(n-1)*a(n-2)/a(n-3), with a(0)=a(1)=1.
 * https://oeis.org/A060656
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2^⌊n^2/4⌋, a(0)=1, a(1)=1
 * Range:     u8 n <= 5, u16 n <= 7, u32 n <= 11, u64 n <= 15, u128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The recurrence a(n)=2*a(n-1)*a(n-2)/a(n-3) follows from this closed form. */
#include <stdint.h>

seq_t a(idx_t n)
{
    if (n <= 1)
        return (seq_t)1;          /* a(0) = a(1) = 1 */

    /* Compute floor(n^2/4).  n is small enough that n*n fits in idx_t. */
    idx_t exp = (n * n) / 4;    /* exponent of 2 */

    /* Compute 2^exp.  For built‑in types it is safe up to the bit width;
       for arbitrary precision types the multiplication grows as needed. */
    seq_t result = 1;
    for (idx_t i = 0; i < exp; ++i)
        result *= (seq_t)2;

    return result;
}
