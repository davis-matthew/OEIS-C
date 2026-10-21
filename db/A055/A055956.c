/*
 * A055956  n + reversal of base 8 digits of n (written in base 10).
 * https://oeis.org/A055956
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = n + reverse_base8(n)
 * Range:     u8 n <= 66, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   reverse_base8
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/reverse_base8.h"
/* Computes the sequence A055956: n + reverse of the octal digits of n,
   where the reverse is interpreted as a base‑10 integer.  */

seq_t
a(idx_t n)
{
    seq_t rev = reverse_base8(n);           /* reversed octal value      */
    return (seq_t)n + rev;                  /* n + reversed value        */
}
