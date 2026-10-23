/*
 * A170752  Expansion of g.f.: (1+x)/(1-32*x).
 * https://oeis.org/A170752
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0) = 1; for n > 0, a(n) = 33 * 32^(n-1)
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 12, u128 n <= 25,
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

    seq_t pow = seq_pow((seq_t)32, n - 1);   /* compute 32^(n-1)  */
    seq_t res = seq_mul((seq_t)33, pow);    /* multiply by 33    */
    return res;
}
