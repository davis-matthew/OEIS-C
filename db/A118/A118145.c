/*
 * A118145  Start with 1 and repeatedly reverse the digits and add 47 to get
 *          the next term.
 * https://oeis.org/A118145
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn, less
 * Formula:   a(n) = rev(a(n-1)) + 47, a(1)=1, rev(x) = reverse of the decimal
 *            digits of x
 * Range:     u8 n <= 4, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

/* Reverse the decimal digits of a seq_t value. */
static seq_t rev_digits(seq_t x)
{
    seq_t rev = 0;
    while (x != 0)
    {
        rev = rev * 10 + (x % 10);
        x /= 10;
    }
    return rev;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined, but safe */
    seq_t val = (seq_t)1;          /* a(1) */
    for (idx_t i = 2; i <= n; ++i)
    {
        seq_t rev = rev_digits(val);
        val = rev + (seq_t)47;    /* a(i) = rev(a(i-1)) + 47 */
    }
    return val;
}
