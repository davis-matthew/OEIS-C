/*
 * A212333  n-th power of the n-th pentagonal number.
 * https://oeis.org/A212333
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n*(3n-1)/2)^n
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 5, u64 n <= 9, u128 n <= 15,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the n-th power of the n-th pentagonal number using
   the provided big_pow utility for arbitrary precision. */

#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;          /* 0^0 is defined as 1 for this sequence */

    /* Convert n to seq_t for high‑precision arithmetic */
    seq_t tn  = (seq_t)n;
    seq_t pent = tn * ((seq_t)3 * tn - 1) / 2;   /* n*(3n-1)/2 */

    return big_pow(pent, tn);   /* pent^n */
}
