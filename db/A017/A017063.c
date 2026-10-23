/*
 * A017063  a(n) = (7*n + 6)^11.
 * https://oeis.org/A017063
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (7*n + 6)^11
 * Range:     u8 none, u16 none, u32 n <= 0, u64 n <= 7, u128 n <= 453,
 *            big any n
 * Helpers:   seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/seq_pow.h"

seq_t a(idx_t n)
{
    /* Compute 7*n + 6 as a seq_t value */
    seq_t base = (seq_t)7 * (seq_t)n + (seq_t)6;
    /* Raise to the 11th power using the provided seq_pow helper */
    return seq_pow(base, 11);
}
