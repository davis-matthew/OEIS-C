/*
 * A021415  Decimal expansion of 1/411.
 * https://oeis.org/A021415
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons, easy
 * Formula:   a(n) = floor((10^n mod 411) * 10 / 411)
 * Range:     u8 n <= 2, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) gives the n-th digit after the decimal point in the expansion of 1/411. */
#include "../utils/mod_pow.h"

seq_t a(idx_t n)
{
    /* Compute r = 10^n mod 411. */
    seq_t r = mod_pow((seq_t)10, n, (seq_t)411);
    /* The digit is given by floor((r * 10) / 411). */
    return (r * 10) / 411;
}
