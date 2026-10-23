/*
 * A190567  Continued fraction expansion of 46*sqrt(46).
 * https://oeis.org/A190567
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy, mult
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A190567:  a(0) = 311;
   for n > 0:  a(n) = 622  if n ≡ 0 (mod 4)   (and n ≥ 4);
               a(n) = 76   if n ≡ 2 (mod 4);
               a(n) = 1    otherwise. */
#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)311;

    switch (n & (idx_t)3)  /* n % 4 */
    {
        case 0:  /* 4k with k ≥ 1 */
            return (seq_t)622;
        case 2:
            return (seq_t)76;
        case 1:
        case 3:
            return (seq_t)1;
    }
    /* Unreachable, but keep compiler happy */
    return (seq_t)0;
}
