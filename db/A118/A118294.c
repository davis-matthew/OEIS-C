/*
 * A118294  Start with 19 and repeatedly reverse the digits and add 1 to get
 *          the next term.
 * https://oeis.org/A118294
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(1) = 19, a(n) = reverse_decimal(a(n-1)) + 1 (n > 1)
 * Range:     u8 n <= 99, u16 n <= 99, u32 n <= 99, u64 n <= 99, u128 n <= 99,
 *            big any n
 * Helpers:   reverse_decimal
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/reverse_decimal.h"

seq_t a(idx_t n)
{
    /* offset starts at 1, so n == 1 returns the initial value */
    seq_t val = (seq_t)19;
    for (idx_t i = 2; i <= n; ++i)
    {
        val = reverse_decimal(val) + 1;
    }
    return val;
}
