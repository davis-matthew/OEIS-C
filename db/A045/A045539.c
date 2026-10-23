/*
 * A045539  Multiply by 5 and reverse.
 * https://oeis.org/A045539
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = reverse(5 * a(n-1)), a(0) = 1
 * Range:     u8 n <= 2, u16 n <= 18, u32 n <= 37, u64 n <= 77, u128 n <= 168,
 *            big any n
 * Helpers:   _reverse_seq_digits
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/_reverse_seq_digits.h"

seq_t a(idx_t n)
{
    seq_t val = 1;          /* a(0) */
    for (idx_t i = 0; i < n; ++i) {
        val = val * 5;      /* multiply by 5 */
        val = _reverse_seq_digits(val); /* reverse decimal digits */
    }
    return val;
}
