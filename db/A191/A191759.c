/*
 * A191759  Least significant decimal digit of (2n-1)^2.
 * https://oeis.org/A191759
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, base
 * Formula:   a(n) = (2n-1)^2 mod 10
 * Range:     u8 n <= 87, u16 n <= 87, u32 n <= 87, u64 n <= 87, u128 n <= 87,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute the least significant decimal digit of the square of the
   nth odd integer.  The sequence is periodic with period 5, so the
   calculation reduces to a few modular arithmetic operations on small
   integers. */
#include <stdint.h>

seq_t a(idx_t n)
{
    /* n is an unsigned 64‑bit integer; we only need n mod 5. */
    idx_t r = n % 5;               /* 0 <= r < 5 */

    /* Compute (2*n - 1) mod 10 efficiently: 2*n mod 10 depends only on
       n mod 5.  Subtract 1 modulo 10 by adding 9. */
    unsigned int t = (unsigned)((r * 2 + 9) % 10);   /* 0 <= t < 10 */

    /* The result is t^2 mod 10.  t^2 fits in an unsigned int. */
    unsigned int res = (t * t) % 10;

    return (seq_t)res;              /* cast back to the generic seq_t */
}
