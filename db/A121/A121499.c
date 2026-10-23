/*
 * A121499  Denominators of partial alternating sums of Catalan numbers scaled
 *          by powers of 1/(29^2) = 1/841.
 * https://oeis.org/A121499
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   a(n) = 841^n
 * Range:     u8 n <= 0, u16 n <= 1, u32 n <= 3, u64 n <= 6, u128 n <= 10,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    /* 841 = 29^2 is the common ratio of the denominators.  The
       partial sums of the alternating Catalan series reduce to the
       fraction with denominator 841^n in lowest terms for every n.  
       Hence a(n) is simply 841 raised to the power n. */
    return big_pow((seq_t)841, n);
}
