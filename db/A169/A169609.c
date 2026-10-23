/*
 * A169609  Period 3: repeat [1, 3, 3].
 * https://oeis.org/A169609
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, cofr, cons, nonn
 * Formula:   a(n) = a(n-3) (period 3) Initial values: a(0)=1, a(1)=3, a(2)=3
 *            Equivalent: a(n) = 1 if n mod 3 == 0, otherwise 3
 * Range:     u8 n <= 104, u16 n <= 104, u32 n <= 104, u64 n <= 104,
 *            u128 n <= 104, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

seq_t a(idx_t n)
{
    /* compute n mod 3 */
    idx_t r = n % (idx_t)3;
    /* return the corresponding value */
    return (r == 0) ? (seq_t)1 : (seq_t)3;
}
