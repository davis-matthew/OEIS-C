/*
 * A118617  Start with 1 and repeatedly reverse the digits and add 31 to get
 *          the next term.
 * https://oeis.org/A118617
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn, less, easy
 * Formula:   a(1) = 1; for n>1: a(n) = rev_decimal(a(n-1)) + 31
 * Range:     u8 n <= 66, u16 n <= 66, u32 n <= 66, u64 n <= 66, u128 n <= 66,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static seq_t reverse_decimal(seq_t x)
{
    seq_t r = 0;
    while (x > 0) {
        r = r * 10 + x % 10;
        x = x / 10;
    }
    return r;
}

seq_t a(idx_t n)
{
    if (n == 1)
        return (seq_t)1;

    seq_t val = (seq_t)1;
    for (idx_t i = 2; i <= n; ++i)
        val = reverse_decimal(val) + (seq_t)31;
    return val;
}
