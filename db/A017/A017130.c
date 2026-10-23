/*
 * A017130  a(n) = (8*n + 5)^6.
 * https://oeis.org/A017130
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (8*n + 5)^6 OEIS A017130: a(n) = (8*n + 5)^6, Offset: 0
 * Range:     u8 none, u16 n <= 0, u32 n <= 4, u64 n <= 202, u128 n <= 10000,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    /* Compute the base (8*n + 5) in the seq_t type. */
    seq_t base = (seq_t)8 * n + 5;

    /* Raise to the 6th power using the provided big integer power
       routine which works for any seq_t size. */
    return big_pow(base, 6);
}
