/*
 * A013737  a(n) = 5^(3*n + 2).
 * https://oeis.org/A013737
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 5^(3*n + 2)
 * Range:     u8 n <= 0, u16 n <= 1, u32 n <= 3, u64 n <= 8, u128 n <= 17,
 *            big any n
 * Helpers:   seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/seq_pow.h"

/* Computes a(n) = 5^(3*n + 2) */
seq_t a(idx_t n)
{
    seq_t base = (seq_t)5;
    idx_t exp = 3*n + 2;
    return seq_pow(base, exp);
}
