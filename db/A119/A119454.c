/*
 * A119454  Start with 34 and repeatedly reverse the digits and add 16 to get
 *          the next term.
 * https://oeis.org/A119454
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 54, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Validated: partial, pass, 54 terms through n = 54 (2026-11-01)
 * Helpers:   reverse_digits
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(1) = 34, a(n) = reverse(a(n-1)) + 16 for n > 1 */
#include <stddef.h>
#include <stdlib.h>
#include "../utils/reverse_digits.h"

seq_t a(idx_t n)
{
    if (n < 1)
        return 0;   /* behavior for indices below offset is irrelevant */

    seq_t x = 34;  /* a(1) */
    for (idx_t i = 1; i < n; ++i)
        x = reverse_digits(x) + (seq_t)16;
    return x;
}
