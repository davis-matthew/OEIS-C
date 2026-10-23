/*
 * A166384  a(n) = (5*n)!/(2*n)!.
 * https://oeis.org/A166384
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (5*n)!/(2*n)!
 * Range:     u8 n <= 1, u16 n <= 1, u32 n <= 3, u64 n <= 5, u128 n <= 8,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

seq_t a(idx_t n)
{
    seq_t result = 1;
    idx_t start = 2*n + 1;
    idx_t end   = 5*n;

    for (idx_t k = start; k <= end; ++k)
    {
        result *= (seq_t)k;
    }
    return result;
}
