/*
 * A132433  a(1) = 2; for n>=2, a(n) = 8*a(n-1) + 1.
 * https://oeis.org/A132433
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (15 * 8^(n-1) - 1) / 7
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 21, u128 n <= 42,
 *            big any n
 * Helpers:   seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/seq_pow.h"

seq_t a(idx_t n)
{
    /* 8^(n-1)  */
    seq_t pow = seq_pow((seq_t)8, n - 1);

    /* (15 * 8^(n-1) - 1) / 7  */
    seq_t tmp = ((seq_t)15 * pow) - ((seq_t)1);
    return tmp / ((seq_t)7);
}
