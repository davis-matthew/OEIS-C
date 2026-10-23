/*
 * A124647  a(n) = (2n + 1)*3^n.
 * https://oeis.org/A124647
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (2*n + 1) * 3^n
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 16, u64 n <= 36, u128 n <= 76,
 *            big any n
 * Helpers:   pow3
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Uses the helper function pow3 from the utils library. */

#include "../utils/pow3.h"

seq_t a(idx_t n)
{
    /* Compute (2*n + 1) as a seq_t value. */
    seq_t mult = ((seq_t)n) * 2 + 1;

    /* Compute 3^n using the provided helper. */
    seq_t pow3n = pow3(n);

    /* Return the product. */
    return mult * pow3n;
}
