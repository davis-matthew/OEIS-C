/*
 * A125831  a(n) = (5^n - 1)/2.
 * https://oeis.org/A125831
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = (5^n - 1)/2 Equivalent recurrence: a(0)=0,
 *            a(n)=5*a(n-1)+2
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 13, u64 n <= 27, u128 n <= 55,
 *            big any n
 * Helpers:   power5
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/power5.h"

seq_t a(idx_t n)
{
    seq_t r = power5(n);  /* r = 5^n */
    r -= 1;               /* r = 5^n - 1 */
    r /= 2;               /* r = (5^n - 1)/2 */
    return r;
}
