/*
 * A169392  Number of reduced words of length n in Coxeter group on 43
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^31 = I.
 * https://oeis.org/A169392
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1; for n ≥ 1, a(n) = 43 * 42^(n-1) (number of reduced
 *            words)
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 11, u128 n <= 14,
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

    seq_t pow42 = (seq_t)1;
    for (idx_t i = 1; i < n; ++i) {
        pow42 *= (seq_t)42;
    }
    return pow42 * (seq_t)43;
}
