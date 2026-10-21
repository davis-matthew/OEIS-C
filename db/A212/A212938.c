/*
 * A212938  Number of n X 3 0..2 arrays with no column j greater than or equal
 *          to than column j-1 in all rows.
 * https://oeis.org/A212938
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 10^n - 2*18^n + 27^n
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 6, u64 n <= 13, u128 n <= 26,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;   /* terms are defined for n ≥ 1 */

    seq_t p10 = big_pow((seq_t)10, (seq_t)n);
    seq_t p18 = big_pow((seq_t)18, (seq_t)n);
    seq_t p27 = big_pow((seq_t)27, (seq_t)n);

    seq_t result = p10;
    result -= (seq_t)2 * p18;
    result += p27;

    return result;
}
