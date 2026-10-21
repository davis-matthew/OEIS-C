/*
 * A063051  'Reverse and Add!' trajectory of 879.
 * https://oeis.org/A063051
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(0) = 879; a(n) = a(n-1) + reverse(a(n-1)) for n ≥ 1
 * Range:     u8 none, u16 n <= 3, u32 n <= 14, u64 n <= 37, u128 n <= 84,
 *            big any n
 * Helpers:   reverse_digits
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/reverse_digits.h"

seq_t a(idx_t n)
{
    seq_t val = (seq_t)879;          /* a(0) */
    if (n == 0) return val;

    for (idx_t i = 0; i < n; ++i) {
        seq_t rev = reverse_digits(val); /* compute reverse of current value */
        val = val + rev;                 /* next term in trajectory */
    }
    return val;
}
