/*
 * A118619  Start with 1 and repeatedly reverse the digits and add 33 to get
 *          the next term.
 * https://oeis.org/A118619
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn, easy
 * Formula:   a(1)=1; for n>1, a(n)=reverse_digits(a(n-1))+33
 * Range:     u8 n <= 63, u16 n <= 63, u32 n <= 63, u64 n <= 63, u128 n <= 63,
 *            big any n
 * Helpers:   reverse_digits
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/reverse_digits.h"

seq_t a(idx_t n)
{
    seq_t val = 1;
    if (n <= 1) return val;
    for (idx_t i = 2; i <= n; ++i)
        val = reverse_digits(val) + (seq_t)33;
    return val;
}
