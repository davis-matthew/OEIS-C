/*
 * A056992  Digital roots of square numbers A000290.
 * https://oeis.org/A056992
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = 1 + ((n*n - 1) mod 9)
 * Range:     u8 n <= 5000, u16 n <= 5000, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Digital root of n^2; uses the fact that digital root mod 9 is
       equivalent to the value modulo 9, with 0 mapped to 9.  */
    seq_t r = n % 9;          /* n mod 9 */
    r = (r * r) % 9;          /* (n^2) mod 9 */
    seq_t diff = (r + 8) % 9; /* (r - 1) mod 9, avoiding signed wrap */
    return 1 + diff;          /* 1..9 inclusive */
}
