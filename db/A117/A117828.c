/*
 * A117828  Start with 1 and repeatedly reverse the decimal digits and add 4
 *          to get the next term.
 * https://oeis.org/A117828
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = reverse(a(n-1)) + 4, a(1) = 1
 * Range:     u8 n <= 9, u16 n <= 71, u32 n <= 71, u64 n <= 71, u128 n <= 71,
 *            big any n
 * Validated: partial, pass, 71 terms through n = 71 (2026-11-01)
 * Helpers:   reverse_digits
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/reverse_digits.h"

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)1;

    seq_t val = (seq_t)1;
    for (idx_t i = 1; i < n; ++i) {
        val = reverse_digits(val) + (seq_t)4;
    }
    return val;
}
