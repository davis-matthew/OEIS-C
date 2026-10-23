/*
 * A013838  a(n) = 6^(5*n + 1).
 * https://oeis.org/A013838
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 6^(5*n + 1)
 * Range:     u8 n <= 0, u16 n <= 1, u32 n <= 2, u64 n <= 4, u128 n <= 9,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    /* Compute exponent = 5*n + 1 in seq_t (fits in 64‑bit for all tested n) */
    seq_t exp = (seq_t)(5*n + 1);
    /* Base 6 as seq_t */
    seq_t base = (seq_t)6;
    /* Use the provided generic big_pow implementation */
    return big_pow(base, exp);
}
