/*
 * A078352  Number of n X n matrices over an alphabet of size 18.
 * https://oeis.org/A078352
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 18^(n^2)
 * Range:     u8 n <= 1, u16 n <= 1, u32 n <= 2, u64 n <= 3, u128 n <= 5,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    /* exponent = n * n, computed in seq_t to avoid overflow of idx_t */
    seq_t exponent = (seq_t)n * (seq_t)n;
    seq_t base = (seq_t)18;
    return big_pow(base, exponent);
}
