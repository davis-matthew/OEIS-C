/*
 * A192736  Right edge of the triangle in A033291.
 * https://oeis.org/A192736
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n * A007980(n-1), where A007980(0) = 1, A007980(k) =
 *            A007980(k-1) + floor((2*k+3)/3) for k ≥ 1. This reproduces the
 *            OEIS sequence A192736
 * Range:     u8 n <= 8, u16 n <= 44, u32 n <= 44, u64 n <= 44, u128 n <= 44,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;
    seq_t val = 1; /* A007980(0) */
    for (idx_t k = 1; k < n; ++k)
    {
        val += (seq_t)((2 * k + 3) / 3);
    }
    return (seq_t)n * val;
}
