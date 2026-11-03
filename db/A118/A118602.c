/*
 * A118602  Start with 1 and repeatedly reverse the digits and add 21 to get
 *          the next term.
 * https://oeis.org/A118602
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(1) = 1, a(n) = reverse_decimal(a(n-1)) + 21 for n > 1
 * Range:     u8 n <= 7, u16 n <= 56, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Validated: partial, pass, 56 terms through n = 56 (2026-11-01)
 * Helpers:   reverse_digits
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/reverse_digits.h"

seq_t a(idx_t n)
{
    seq_t val = 1;             /* a(1) */
    if (n <= 1)                /* covers n == 1 */
        return val;

    for (idx_t i = 2; i <= n; ++i) {
        seq_t rev = reverse_digits(val);
        val = rev + (seq_t)21;
    }
    return val;
}
