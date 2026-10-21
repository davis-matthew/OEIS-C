/*
 * A099544  Odd part of n modulo 3.
 * https://oeis.org/A099544
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = A010872(A000265(n)) (odd part of n modulo 3)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Computes a(n) for n >= 1:
   0  if n has a factor 3 in its odd part,
   1  if the odd part of n is congruent to 1 mod 3,
   2  if the odd part of n is congruent to 2 mod 3. */
seq_t a(idx_t n)
{
    /* Remove all factors of 2 to get the odd part */
    while ((n & 1) == 0)
        n >>= 1;

    /* Now n is odd */
    seq_t mod3 = (seq_t)(n % 3);
    return mod3;   /* 0, 1, or 2 */
}
