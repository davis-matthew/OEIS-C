/*
 * A016943  a(n) = (6*n + 2)^11.
 * https://oeis.org/A016943
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (6*n + 2)^11
 * Range:     u8 none, u16 n <= 0, u32 n <= 0, u64 n <= 9, u128 n <= 530,
 *            big any n
 * Helpers:   seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/seq_pow.h"

seq_t a(idx_t n)
{
    /* Convert the index to seq_t before any multiplication to avoid
       truncation of large idx_t values. */
    seq_t base = ((seq_t)6 * (seq_t)n) + (seq_t)2;
    return seq_pow(base, 11);
}
