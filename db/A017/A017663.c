/*
 * A017663  a(n) = (12*n + 11)^11.
 * https://oeis.org/A017663
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (12*n + 11)^11
 * Range:     u8 none, u16 none, u32 none, u64 n <= 3, u128 n <= 264,
 *            big any n
 * Helpers:   seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/seq_pow.h"

seq_t a(idx_t n)
{
    /* Convert the index to the base value */
    seq_t base = (seq_t)n * 12 + 11;
    /* Raise to the 11th power */
    return seq_pow(base, 11);
}
