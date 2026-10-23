/*
 * A169451  Number of reduced words of length n in Coxeter group on 6
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^33 = I.
 * https://oeis.org/A169451
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1 a(n) = 6 * 5^(n-1) for 1 ≤ n ≤ 21 a(n) = 0 for n > 21
 *            This is the number of reduced words of length n in the Coxeter
 *            group defined by 6 generators S_i, (S_i)^2 = I, (S_i S_j)^3 = I.
 *            The maximum length in S_7 is 21, so all other terms vanish
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 13, u64 n <= 21, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;
    if (n > 21)
        return (seq_t)0;

    seq_t power = (seq_t)1;          /* 5^(n-1) will be computed here. */
    for (idx_t i = 1; i < n; ++i)    /* Run n-1 times. */
        power *= (seq_t)5;

    return power * (seq_t)6;
}
