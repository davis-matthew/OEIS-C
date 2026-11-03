/*
 * A160924  a(n)= n + reversal(n+1).
 * https://oeis.org/A160924
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = n + reverse(n+1)
 * Range:     u8 n <= 65, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Validated: partial, pass, 65 terms through n = 65 (2026-11-01)
 * Helpers:   reverse_digits_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/reverse_digits_idx.h"

seq_t a(idx_t n)
{
    /* Compute the decimal reversal of n+1 and add n. */
    idx_t r = reverse_digits_idx(n + 1);          /* r = reverse(n+1) */
    return (seq_t)n + (seq_t)r;    /* result in seq_t */
}
