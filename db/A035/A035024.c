/*
 * A035024  Expansion of 1/(1-81*x)^(1/9), related to 9-factorial numbers
 *          A045756.
 * https://oeis.org/A035024
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 5, u64 n <= 11, u128 n <= 21,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0) = 1,
   n*a(n) = 9*(9*n-8)*a(n-1) for n >= 1 */
#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;

    seq_t val = (seq_t)1;
    for (idx_t i = 1; i <= n; ++i)
    {
        seq_t i_seq = (seq_t)i;

        /* mult = 9 * (9*i - 8) */
        seq_t mult = (seq_t)9 * (((seq_t)9 * i_seq) - (seq_t)8);

        /* Reduce by gcd(val, i) */
        seq_t g1 = gcd(val, i_seq);
        seq_t val_div = val / g1;
        seq_t i1 = i_seq / g1;

        /* Reduce by gcd(mult, i1) */
        seq_t g2 = gcd(mult, i1);
        seq_t mult_div = mult / g2;
        seq_t i2 = i1 / g2;

        /* i2 divides val_div; divide first to avoid overflow */
        seq_t val_div2 = val_div / i2;      /* exact division */

        val = val_div2 * mult_div;
    }
    return val;
}
