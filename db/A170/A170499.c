/*
 * A170499  Number of reduced words of length n in Coxeter group on 10
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^46 = I.
 * https://oeis.org/A170499
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1; a(n) = 10 * 9^(n-1) for n ≥ 1
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 10, u64 n <= 18, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0) {
        return (seq_t)1;
    }
    /* Start with the constant 10 and multiply by 9 n-1 times */
    seq_t result = (seq_t)10;
    for (idx_t i = 1; i < n; ++i) {
        result *= (seq_t)9;
    }
    return result;
}
