/*
 * A168959  Number of reduced words of length n in Coxeter group on 42
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^22 = I.
 * https://oeis.org/A168959
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1, a(n) = 42 * 41^(n-1) for n ≥ 1 * (number of reduced
 *            words of length n in the free product of 42 copies of C2)
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

    idx_t exp = n - 1;
    seq_t base = (seq_t)41;
    seq_t result = (seq_t)1;

    while (exp > 0) {
        if (exp & (idx_t)1)
            result *= base;
        base *= base;
        exp >>= 1;
    }
    return result * (seq_t)42;
}
