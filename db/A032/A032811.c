/*
 * A032811  Numbers whose set of base-11 digits is {2,3}.
 * https://oeis.org/A032811
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 62, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: Numbers whose base‑11 digits are {2,3}
   Recurrence:  a(1)=2, a(2)=3;
   a(n) = 11*a(n/2) + 2  if n is odd;
   a(n) = 11*a((n-1)/2) + 3  if n is even; */
#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 1) return 2;
    if (n == 2) return 3;
    if (n & 1)   /* n odd */
        return (seq_t)11 * a(n / 2) + 2;
    else         /* n even */
        return (seq_t)11 * a((n - 1) / 2) + 3;
}
