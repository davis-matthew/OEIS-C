/*
 * A137704  Hankel transform of aerated factorial numbers.
 * https://oeis.org/A137704
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = Product_{k=0..n} floor((k+2)/2)^(n-k)
 * Range:     u8 n <= 5, u16 n <= 6, u32 n <= 8, u64 n <= 10, u128 n <= 13,
 *            big any n
 * Helpers:   big_pow, seq_mul, seq_one
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"
#include "../utils/seq_mul.h"
#include "../utils/seq_one.h"

seq_t a(idx_t n)
{
    /* a(0) = 1 */
    if (n == 0)
        return seq_one();

    seq_t result = seq_one();

    /* t runs from 0 to floor((n-1)/2).  For n>=1 this is valid. */
    idx_t t_max = (n - 1) / 2;

    for (idx_t t = 0; t <= t_max; ++t)
    {
        seq_t base    = (seq_t)(t + 1);        /* floor(k/2)+1 == t+1 */
        /* Exponent: e = 2*n - 4*t - 1  (always positive for n>=1) */
        idx_t exp_idx = (2 * n) - (4 * t) - 1;
        seq_t exp     = (seq_t)exp_idx;

        seq_t pow_val = big_pow(base, exp);
        result = seq_mul(result, pow_val);
    }

    return result;
}
