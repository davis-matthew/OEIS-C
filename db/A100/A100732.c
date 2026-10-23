/*
 * A100732  a(n) = (3*n)!.
 * https://oeis.org/A100732
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (3*n)!
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 4, u64 n <= 6, u128 n <= 11,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    idx_t m = 3 * n;
    seq_t result = 1;
    for (idx_t i = 1; i <= m; ++i) {
        result *= (seq_t)i;
    }
    return result;
}
