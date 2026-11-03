/*
 * A167542  Natural numbers, swapped in pairs, with decimal digits reversed.
 * https://oeis.org/A167542
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = reverse_decimal( (n%2? n+1 : n-1) )
 * Range:     u8 n <= 75, u16 n <= 75, u32 n <= 75, u64 n <= 75, u128 n <= 75,
 *            big any n
 * Validated: partial, pass, 75 terms through n = 75 (2026-11-01)
 * Helpers:   reverse_digits
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/reverse_digits.h"

seq_t a(idx_t n)
{
    /* swap the pair that contains n */
    idx_t swapped = (n & (idx_t)1) ? n + 1 : n - 1;
    /* reverse the decimal digits of the swapped number */
    seq_t x = (seq_t)swapped;
    return reverse_digits(x);
}
