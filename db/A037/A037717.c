/*
 * A037717  Base 5 digits are, in order, the first n terms of the periodic
 *          sequence with initial period 2,0,1,3.
 * https://oeis.org/A037717
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(1)=2, a(n) = 5*a(n-1) + d(n) where d(n) repeats the pattern
 *            2,0,1,3 (i.e. d(1)=2, d(2)=0, d(3)=1, d(4)=3, d(5)=2, …)
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 14, u64 n <= 28, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0; /* undefined for n<1, return 0 for safety */

    seq_t result = (seq_t)0;
    for (idx_t i = 1; i <= n; ++i)
    {
        /* compute the repeating digit */
        idx_t r = (i - 1) & 3u; /* same as (i-1)%4 but faster on powers of two */
        seq_t digit;
        switch (r)
        {
            case 0u: digit = (seq_t)2; break;
            case 1u: digit = (seq_t)0; break;
            case 2u: digit = (seq_t)1; break;
            default: digit = (seq_t)3; break; /* r==3 */
        }
        result = result * (seq_t)5 + digit;
    }
    return result;
}
