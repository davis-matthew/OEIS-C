/*
 * A054243  Number of partitions of n into distinct positive parts <= n, where
 *          parts are combined by XOR.
 * https://oeis.org/A054243
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, nice
 * Formula:   a(n) = 2^floor(n - log2(n) - 1) (a(0) = 1)
 * Range:     u8 n <= 11, u16 n <= 20, u32 n <= 37, u64 n <= 70, u128 n <= 135,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Special case for n = 0 */
    if (n == 0)
        return 1;

    /* Compute floor(log2(n)) */
    idx_t floor_log2 = 0;
    idx_t tmp = n;
    while (tmp >>= 1)
        ++floor_log2;

    /* Exponent of 2 */
    idx_t exp = n - floor_log2 - 1;

    /* 2^exp by repeated left shifts (safe for any seq_t size) */
    seq_t result = 1;
    for (idx_t i = 0; i < exp; ++i)
        result <<= 1;

    return result;
}
