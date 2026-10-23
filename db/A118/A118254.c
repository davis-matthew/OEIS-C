/*
 * A118254  Start with 16 and repeatedly reverse the digits and add 1 to get
 *          the next term.
 * https://oeis.org/A118254
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(1) = 16; a(n) = reverse(a(n-1)) + 1 (n > 1)
 * Range:     u8 n <= 96, u16 n <= 96, u32 n <= 96, u64 n <= 96, u128 n <= 96,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

static seq_t reverse_digits(seq_t x)
{
    seq_t rev = 0;
    while (x > 0)
    {
        rev = rev * 10 + (x % 10);
        x = x / 10;
    }
    return rev;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;  /* valid only for n >= 1 as per the OEIS offset */
    seq_t val = 16;
    for (idx_t i = 1; i < n; ++i)
    {
        val = reverse_digits(val) + 1;
    }
    return val;
}
