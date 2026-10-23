/*
 * A115490  Number of monic irreducible polynomials of degree 4 in GF(2^n)[x].
 * https://oeis.org/A115490
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (16^n - 4^n)/4
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 8, u64 n <= 16, u128 n <= 32,
 *            big any n
 * Helpers:   seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/seq_pow.h"

seq_t a(idx_t n)
{
    seq_t pow16 = seq_pow((seq_t)16, n);
    seq_t pow4  = seq_pow((seq_t)4,  n);
    return (pow16 - pow4) / ((seq_t)4);
}
