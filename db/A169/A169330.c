/*
 * A169330  Number of reduced words of length n in Coxeter group on 29
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^30 = I.
 * https://oeis.org/A169330
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1; for n >= 1, a(n) = 29 * 28^(n-1)
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 13, u128 n <= 15,
 *            big any n
 * Helpers:   seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/seq_pow.h"

seq_t a(idx_t n)
{
    if (n == 0) return 1;
    seq_t res = seq_pow((seq_t)28, n - 1); /* 28^(n-1) */
    res *= (seq_t)29;                      /* multiply by 29 */
    return res;
}
