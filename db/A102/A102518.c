/*
 * A102518  a(n) = Sum_{k=0..n} binomial(n, k) * Sum_{j=0..k} binomial(3k,
 *          3j).
 * https://oeis.org/A102518
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 1 (n = 0); a(n) = 3^(2*n-1) for n >= 1
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 10, u64 n <= 20, u128 n <= 21,
 *            big any n
 * Helpers:   pow3
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow3.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;
    idx_t exp = 2 * n - 1;
    return pow3(exp);
}
