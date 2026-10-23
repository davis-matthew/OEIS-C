/*
 * A170275  Number of reduced words of length n in Coxeter group on 26
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^41 = I.
 * https://oeis.org/A170275
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 (n=0) else 26 * 25^(n-1) (n>=1)
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 13, u128 n <= 15,
 *            big any n
 * Helpers:   seq_mul, seq_one, seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/seq_one.h"
#include "../utils/seq_mul.h"
#include "../utils/seq_pow.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return seq_one();

    if (n == 1)
        return (seq_t)26;

    seq_t power = seq_pow((seq_t)25, n - 1);
    return seq_mul((seq_t)26, power);
}
