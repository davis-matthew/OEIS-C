/*
 * A135428  a(n) = 9^n * 10^(n^2).
 * https://oeis.org/A135428
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 9^n * 10^(n^2)
 * Range:     u8 n <= 1, u16 n <= 1, u32 n <= 2, u64 n <= 3, u128 n <= 5,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    /* Convert the index to seq_t before arithmetic */
    seq_t sn = (seq_t)n;

    /* Compute 9^n */
    seq_t p9 = big_pow((seq_t)9, sn);

    /* Compute n^2 in seq_t */
    seq_t exp = sn * sn;

    /* Compute 10^(n^2) */
    seq_t p10 = big_pow((seq_t)10, exp);

    /* Return the product */
    return p9 * p10;
}
