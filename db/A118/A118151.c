/*
 * A118151  Start with 1 and repeatedly reverse the digits and add 54 to get
 *          the next term.
 * https://oeis.org/A118151
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn, less
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 52, u32 n <= 52, u64 n <= 52, u128 n <= 52,
 *            big any n
 * Helpers:   rev_digits_local
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(1) = 1; a(n) = rev(a(n-1)) + 54 for n > 1. */
#include <stdint.h>
#include "../utils/rev_digits_local.h"

seq_t a(idx_t n)
{
    if (n == 1)
        return (seq_t)1;
    seq_t val = 55;            /* a(2) */
    for (idx_t i = 2; i < n; ++i) {
        val = rev_digits_local(val) + (seq_t)54;
    }
    return val;
}
