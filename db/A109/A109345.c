/*
 * A109345  a(n) = 5^((n^2 - n)/2).
 * https://oeis.org/A109345
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 5^((n^2 - n)/2)
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 5, u64 n <= 7, u128 n <= 11,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    /* Compute exponent e = (n^2 - n)/2  -- it always fits in idx_t for n <= 10000 */
    idx_t e = (idx_t)(((seq_t)n * ((seq_t)n - 1)) / 2);

    seq_t base = 5;
    seq_t result = 1;
    while (e > 0) {
        if (e & 1)
            result *= base;
        base *= base;
        e >>= 1;
    }
    return result;
}
