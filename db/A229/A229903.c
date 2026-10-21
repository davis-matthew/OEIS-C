/*
 * A229903  a(n) = (190/99)*(100^A001651(n)-1).
 * https://oeis.org/A229903
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (190/99)*(100^floor((3*n-1)/2)-1)
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 3, u64 n <= 6, u128 n <= 11,
 *            big any n
 * Helpers:   seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/seq_pow.h"

seq_t a(idx_t n)
{
    /* OEIS offset is 1; return 0 when n == 0 for safety. */
    if (n == 0)
        return (seq_t)0;

    /* Compute m = floor((3*n-1)/2) */
    idx_t m = (3 * n - 1) / 2;

    /* Compute 100^m as a seq_t. */
    seq_t val = seq_pow((seq_t)100, m);

    /* Subtract 1, divide by 99 (exact division), then multiply by 190. */
    val -= (seq_t)1;
    val /= (seq_t)99;
    val *= (seq_t)190;

    return val;
}
