/*
 * A170055  Number of reduced words of length n in Coxeter group on 46
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^36 = I.
 * https://oeis.org/A170055
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1; for n ≥ 1, a(n) = 45^n + 45^(n‑1)
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 11, u128 n <= 14,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    if (n == 0) {
        /* Return the integer 1 for all seq_t types. */
        return (seq_t)1;
    }

    seq_t pow_n     = big_pow((seq_t)45, (seq_t)n);
    seq_t pow_prev  = big_pow((seq_t)45, (seq_t)(n - 1));
    return pow_n + pow_prev;
}
