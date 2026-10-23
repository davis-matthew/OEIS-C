/*
 * A170305  Number of reduced words of length n in Coxeter group on 8
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^42 = I.
 * https://oeis.org/A170305
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1, a(n) = 8 * 7^(n-1) for n >= 1 (so a(n) = 8*7^(n-1),
 *            a(0) = 1)
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 11, u64 n <= 19, u128 n <= 19,
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

    /* binary exponentiation of 7^(n-1) */
    idx_t exp = n - 1;
    seq_t base = (seq_t)7;
    seq_t result = (seq_t)1;

    while (exp)
    {
        if (exp & 1)
            result *= base;

        base *= base;
        exp >>= 1;
    }

    return ((seq_t)8) * result;
}
