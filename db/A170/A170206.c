/*
 * A170206  Number of reduced words of length n in Coxeter group on 5
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^40 = I.
 * https://oeis.org/A170206
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 if n = 0 * a(n) = 5*4^(n-1) if n >= 1 * This is derived
 *            from the recurrence a(n) = 4*a(n-1) for n >= 2 * with a(0)=1,
 *            a(1)=5. For n >= 1 the closed form is 5*4^(n-1)
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 23, u128 n <= 23,
 *            big any n
 * Helpers:   seq_mul, seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/seq_pow.h"
#include "../utils/seq_mul.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;
    seq_t pow4 = seq_pow((seq_t)4, n - 1);
    return seq_mul(pow4, (seq_t)5);
}
