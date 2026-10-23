/*
 * A013868  a(n) = 13^(5*n + 3).
 * https://oeis.org/A013868
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 13^(5*n + 3)
 * Range:     u8 none, u16 n <= 0, u32 n <= 1, u64 n <= 2, u128 n <= 6,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    /* Compute exponent 5*n + 3 as seq_t to avoid intermediate overflow. */
    seq_t exp = 5 * (seq_t)n + 3;
    /* Compute 13^exp using the provided big integer power routine. */
    return big_pow((seq_t)13, exp);
}
