/*
 * A118614  Start with 1 and repeatedly reverse the digits and add 28 to get
 *          the next term.
 * https://oeis.org/A118614
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn, easy
 * Formula:   a(1) = 1; a(n) = rev(a(n‑1)) + 28 (for n > 1) where rev(x) is
 *            the decimal reversal of x
 * Range:     u8 n <= 6, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   reverse_decimal_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include "../utils/reverse_decimal_seq.h"

/* Sequence A118614.  */
seq_t a(idx_t n)
{
    seq_t val = 1;
    if (n == 1) return val;
    for (idx_t i = 2; i <= n; ++i)
    {
        seq_t rev = reverse_decimal_seq(val);
        val = rev + (seq_t)28;
    }
    return val;
}
