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
 * Helpers:   rev10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/rev10.h"

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)1;

    seq_t val = (seq_t)1;
    for (idx_t i = 1; i < n; ++i) {
        val = rev10(val) + (seq_t)4;
    }
    return val;
}
