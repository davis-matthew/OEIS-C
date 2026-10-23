/*
 * A102041  a(n) = a(n-1) + last digit of a(n-1), starting at 7.
 * https://oeis.org/A102041
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = a(n-1) + lastdigit(a(n-1)), a(1)=7
 * Range:     u8 n <= 50, u16 n <= 56, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

seq_t a(idx_t n)
{
    if (n < 1)
        return 0;        /* undefined for n<1; not used in tests */

    seq_t val = 7;        /* a(1) */
    if (n == 1)
        return val;

    for (idx_t i = 2; i <= n; ++i)
    {
        val += (val % 10);   /* last decimal digit of val */
    }
    return val;
}
