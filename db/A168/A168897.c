/*
 * A168897  Number of reduced words of length n in Coxeter group on 28
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^21 = I.
 * https://oeis.org/A168897
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1; for n ≥ 1, a(n) = 28 × 27^(n‑1)
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 13, u128 n <= 15,
 *            big any n
 * Helpers:   seq_mul, seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Include utility functions for power and multiplication of seq_t.  */
#include "../utils/seq_pow.h"
#include "../utils/seq_mul.h"

seq_t a(idx_t n)
{
    /* The first term is defined as 1.  */
    if (n == (idx_t)0) {
        return (seq_t)1;
    }

    /* a(n) = 28 × 27^(n‑1).  Compute the power and then multiply.  */
    seq_t power_of_27 = seq_pow((seq_t)27, n - (idx_t)1);
    return seq_mul((seq_t)28, power_of_27);
}
