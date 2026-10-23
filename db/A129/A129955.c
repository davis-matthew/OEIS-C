/*
 * A129955  Third differences of A129952.
 * https://oeis.org/A129955
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2, 3 for n = 0, 1; otherwise a(n) = (n + 6) * 2^(n-2)
 * Range:     u8 n <= 6, u16 n <= 13, u32 n <= 28, u64 n <= 59, u128 n <= 122,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)2;
    if (n == 1) return (seq_t)3;

    /* Compute 2^(n-2) by repeated multiplication.
       This works for all allowed seq_t types, including
       an arbitrary-precision integer class that overloads *.
    */
    seq_t pow2 = (seq_t)1;
    idx_t shift = n - 2;
    for (idx_t i = 0; i < shift; ++i)
        pow2 *= (seq_t)2;

    seq_t result = (seq_t)(n + 6) * pow2;
    return result;
}
