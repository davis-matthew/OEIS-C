/*
 * A082448  Add 1, multiply by 1, add 2, multiply by 2, etc.; start with 4.
 * https://oeis.org/A082448
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0) = 4 For n >= 1: If n is odd: a(n) = a(n-1) + ((n+1)/2) If n
 *            is even: a(n) = a(n-1) * (n/2)
 * Range:     u8 n <= 9, u16 n <= 15, u32 n <= 23, u64 n <= 32, u128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    seq_t val = 4;            /* a(0) */
    for (idx_t i = 1; i <= n; ++i)
    {
        if (i & 1)            /* odd i: add (i+1)/2 */
        {
            val += (seq_t)((i + 1) / 2);
        }
        else                  /* even i: multiply by i/2 */
        {
            val *= (seq_t)(i / 2);
        }
    }
    return val;
}
