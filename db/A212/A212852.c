/*
 * A212852  Number of n X 5 arrays with rows being permutations of 0..4 and no
 *          column j greater than column j-1 in all rows.
 * https://oeis.org/A212852
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = -2*5^n + 3*20^n - 4*60^n + 120^n + 3*30^n - 2*10^n + 1
 *            (Offset: 1)
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 4, u64 n <= 9, u128 n <= 18,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    /* compute all required powers once */
    seq_t p5   = big_pow((seq_t)5, (seq_t)n);
    seq_t p10  = big_pow((seq_t)10, (seq_t)n);
    seq_t p20  = big_pow((seq_t)20, (seq_t)n);
    seq_t p30  = big_pow((seq_t)30, (seq_t)n);
    seq_t p60  = big_pow((seq_t)60, (seq_t)n);
    seq_t p120 = big_pow((seq_t)120, (seq_t)n);

    /* positive part */
    seq_t pos = p120 + 3 * p30 + 3 * p20 + 1;

    /* negative part */
    seq_t neg = 2 * p5 + 4 * p60 + 2 * p10;

    /* result (pos >= neg for n >= 1) */
    return pos - neg;
}
