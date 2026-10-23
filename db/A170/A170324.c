/*
 * A170324  Number of reduced words of length n in Coxeter group on 27
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^42 = I.
 * https://oeis.org/A170324
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=1; for n>=1, a(n)=27*26^(n-1)
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 13, u128 n <= 15,
 *            big any n
 * Helpers:   seq_mul, seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/seq_pow.h"   /* seq_pow(seq_t base, idx_t exp) */
#include "../utils/seq_mul.h"   /* seq_mul(seq_t a, seq_t b) */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;
    /* n >= 1 */
    seq_t pow = seq_pow((seq_t)26, n - 1);
    return seq_mul(pow, (seq_t)27);
}
