/*
 * A093069  a(n) = (2^n + 1)^2 - 2.
 * https://oeis.org/A093069
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (2^n + 1)^2 - 2
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 31, u128 n <= 63,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* For n >= 1, returns the nth term of OEIS A093069. */

#include <stdint.h>

/* The function follows the definition directly.
   For arbitrary‑precision builds seq_t supports << and normal arithmetic.
   For fixed‑width builds the values are only evaluated for n that
   actually fit in seq_t, which is the intended behaviour. */

seq_t a(idx_t n)
{
    /* Compute 2^n */
    seq_t two_n = ((seq_t)1) << n;

    /* (2^n + 1)^2 - 2 */
    seq_t t = two_n + 1;
    t = t * t;
    t -= (seq_t)2;

    return t;
}
