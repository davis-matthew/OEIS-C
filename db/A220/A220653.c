/*
 * A220653  a(n) = n^11 + 11*n + 11^n.
 * https://oeis.org/A220653
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^11 + 11*n + 11^n
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 7, u64 n <= 18, u128 n <= 19,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    seq_t sn = (seq_t)n;   /* index as sequence type */

    /* compute n^11 */
    seq_t n_pow = sn;
    for (idx_t i = 1; i < 11; ++i)
        n_pow *= sn;

    /* compute 11*n */
    seq_t term2 = sn * (seq_t)11;

    /* compute 11^n */
    seq_t term3 = big_pow((seq_t)11, sn);

    return n_pow + term2 + term3;
}
