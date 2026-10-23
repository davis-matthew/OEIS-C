/*
 * A008624  Expansion of g.f. (1 + x^3)/((1 - x^2)*(1 - x^4)) = (1 - x +
 *          x^2)/((1 + x)*(1 - x)^2*(1 + x^2)).
 * https://oeis.org/A008624
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor(n/4) + ((n mod 2 + 1 - floor((n mod 4)/3)) mod 2)
 * Range:     u8 n <= 266, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Integer division is exact floor division for non‑negative n. */
    seq_t q = (seq_t)n / 4;          /* floor(n/4) */
    idx_t r = n % 4;                 /* n mod 4, 0 <= r < 4 */

    /* Compute t = ((r mod 2) + 1 - floor(r/3)) mod 2.
       floor(r/3) is 0 for r=0,1,2 and 1 for r=3.  r mod 2 is 0 or 1. */
    seq_t t = ((((seq_t)(r % 2)) + 1) - (seq_t)(r / 3)) & 1;

    return q + t;
}
