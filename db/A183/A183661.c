/*
 * A183661  Number of (n+1) X 9 0..5 arrays with every 2 X 2 subblock summing
 *          to 10.
 * https://oeis.org/A183661
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 6*6^n + 2550*5^n + 74640*4^n + 669870*3^n + 2897040*2^n +
 *            6433590
 * Range:     u8 none, u16 none, u32 n <= 7, u64 n <= 22, u128 n <= 48,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    seq_t exp = (seq_t)n;

    seq_t pow2 = big_pow((seq_t)2, exp);
    seq_t pow3 = big_pow((seq_t)3, exp);
    seq_t pow4 = big_pow((seq_t)4, exp);
    seq_t pow5 = big_pow((seq_t)5, exp);
    seq_t pow6 = big_pow((seq_t)6, exp);

    seq_t result = pow6 * (seq_t)6
                 + pow5 * (seq_t)2550
                 + pow4 * (seq_t)74640
                 + pow3 * (seq_t)669870
                 + pow2 * (seq_t)2897040
                 + (seq_t)6433590;

    return result;
}
