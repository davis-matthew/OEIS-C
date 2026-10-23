/*
 * A169334  Number of reduced words of length n in Coxeter group on 33
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^30 = I.
 * https://oeis.org/A169334
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1, a(n) = 33*32^(n-1) for n >= 1
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 12, u128 n <= 15,
 *            big any n
 * Helpers:   seq_mul, seq_one, seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/seq_pow.h"
#include "../utils/seq_mul.h"
#include "../utils/seq_one.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return seq_one();

    /* Compute 32^(n-1) */
    seq_t power = seq_pow((seq_t)32, n - 1);

    /* Multiply by 33 to get the answer */
    return seq_mul(power, (seq_t)33);
}
