/*
 * A170006  Number of reduced words of length n in Coxeter group on 45
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^35 = I.
 * https://oeis.org/A170006
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1, a(n) = 45 * 44^(n-1) for n ≥ 1
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 11, u128 n <= 14,
 *            big any n
 * Helpers:   seq_mul, seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Number of reduced words of length n in the Coxeter group on 45 generators with (S_i S_j)^35 = I */

/* Include utility headers for exponentiation and multiplication. */
#include "../utils/seq_pow.h"
#include "../utils/seq_mul.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;
    /* Compute 44^(n-1) */
    seq_t pow = seq_pow((seq_t)44, n - 1);
    /* Multiply by 45 */
    return seq_mul(pow, (seq_t)45);
}
