/*
 * A118214  Start with 1 and repeatedly reverse the digits and add 67 to get
 *          the next term.
 * https://oeis.org/A118214
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn, less
 * Formula:   a(1) = 1; for n > 1, a(n) = reverse_decimal(a(n-1)) + 67
 * Range:     u8 n <= 3, u16 n <= 46, u32 n <= 46, u64 n <= 46, u128 n <= 46,
 *            big any n
 * Helpers:   reverse_decimal
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/reverse_decimal.h"

seq_t a(idx_t n)
{
    /* Sequence is defined only for n ≥ 1.  Return 0 for n < 1 as a safe
       default; such calls are outside the specification. */
    if (n < 1)
        return 0;

    seq_t value = 1;          /* a(1) */
    for (idx_t i = 1; i < n; ++i)
        value = reverse_decimal(value) + (seq_t)67;

    return value;
}
