/*
 * A053834  Sum of digits of n written in base 14.
 * https://oeis.org/A053834
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = sum of digits of n written in base 14. Recurrence:
 *            a(0)=0, a(14n+i)=a(n)+i for 0 <= i <= 13
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>  /* for integer types used by the build */

seq_t a(idx_t n)
{
    idx_t  t   = n;   /* use a local variable so that we never modify the argument */
    seq_t  sum = 0;   /* accumulator of the digit sum */

    while (t > 0) {
        sum += (seq_t)(t % 14);  /* add the least significant base‑14 digit */
        t   /= 14;               /* shift right one digit in base 14 */
    }
    return sum;   /* if n == 0 the loop is skipped and sum == 0 as required */
}
