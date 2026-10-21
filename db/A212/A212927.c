/*
 * A212927  Number of n X 5 0..4 arrays with no column j greater than column
 *          j-1 in all rows.
 * https://oeis.org/A212927
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = -2*100^n - 2*25^n - 4*1250^n + 3*500^n + 3*250^n + 3125^n
 *            + 1
 * Range:     u8 n <= 1, u16 n <= 1, u32 n <= 2, u64 n <= 5, u128 n <= 11,
 *            big any n
 * Helpers:   mul_small, seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/seq_pow.h"
#include "../utils/mul_small.h"

seq_t a(idx_t n)
{
    /* Compute required powers of small integers */
    seq_t p100   = seq_pow((seq_t)100,   n);
    seq_t p25    = seq_pow((seq_t)25,    n);
    seq_t p1250  = seq_pow((seq_t)1250,  n);
    seq_t p500   = seq_pow((seq_t)500,   n);
    seq_t p250   = seq_pow((seq_t)250,   n);
    seq_t p3125  = seq_pow((seq_t)3125,  n);

    /* Positive part: 3125^n + 3*500^n + 3*250^n + 1 */
    seq_t res = p3125;
    res += mul_small(p500, (unsigned)3);
    res += mul_small(p250, (unsigned)3);
    res += (seq_t)1;

    /* Negative part: 2*100^n + 2*25^n + 4*1250^n */
    seq_t neg = mul_small(p100, (unsigned)2);
    neg += mul_small(p25,  (unsigned)2);
    neg += mul_small(p1250, (unsigned)4);

    /* Final result: pos - neg */
    res -= neg;

    return res;
}
