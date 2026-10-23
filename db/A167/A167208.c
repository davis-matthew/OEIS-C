/*
 * A167208  Append two digits, each increasing by one modulo 10 from the last
 *          digit of the positive integers. 0 -> 12 1 -> 123 2 -> 234 .. 9 ->
 *          901 10 -> 1012.
 * https://oeis.org/A167208
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(n) = 100*n + 10*((n%10 + 1)%10) + ((n%10 + 2)%10)
 * Range:     u8 n <= 2, u16 n <= 41, u32 n <= 41, u64 n <= 41, u128 n <= 41,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

seq_t a(idx_t n)
{
    /* Convert the index to the sequence type to preserve overflow rules
       and use only integer arithmetic permitted by the spec. */
    seq_t sn = (seq_t)n;
    /* Last digit of n in base 10 */
    seq_t d = sn % 10;
    /* Append the next two digits modulo 10 */
    seq_t next1 = (d + 1) % 10;
    seq_t next2 = (d + 2) % 10;
    /* Form the resulting value */
    return sn * 100 + next1 * 10 + next2;
}
