/*
 * A118264  Coefficient of q^n in (1-q)^3/(1-3q); dimensions of the enveloping
 *          algebra of the derived free Lie algebra on 3 letters.
 * https://oeis.org/A118264
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3^{n-1} - 3^{n-3} for n >= 3; a(0)=1, a(1)=0, a(2)=3
 * Range:     u8 n <= 6, u16 n <= 11, u32 n <= 21, u64 n <= 41, u128 n <= 81,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    /* Handle the initial terms explicitly */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)0;
    if (n == 2) return (seq_t)3;

    /* For n >= 3: a(n) = 3^(n-1) - 3^(n-3) */
    seq_t pow_n_minus_1 = big_pow((seq_t)3, (seq_t)(n - 1));
    seq_t pow_n_minus_3 = big_pow((seq_t)3, (seq_t)(n - 3));
    return pow_n_minus_1 - pow_n_minus_3;
}
