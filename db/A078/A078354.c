/*
 * A078354  Number of n X n matrices over an alphabet of size 20.
 * https://oeis.org/A078354
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 20^(n^2)
 * Range:     u8 n <= 1, u16 n <= 1, u32 n <= 2, u64 n <= 3, u128 n <= 5,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    /* Compute 20^(n^2) using big_pow.
       The exponent is calculated in seq_t to avoid
       overflow of idx_t when n is large. */
    seq_t base = (seq_t)20;
    seq_t exp = (seq_t)n * (seq_t)n;
    return big_pow(base, exp);
}
