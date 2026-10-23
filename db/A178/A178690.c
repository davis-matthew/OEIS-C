/*
 * A178690  Expansion of (exp(3*x)-1)*(exp(2*x)-1)*(exp(x)-1).
 * https://oeis.org/A178690
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 1 + 2^n - 4^n - 5^n + 6^n (a(0) = 0)
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 24, u128 n <= 49,
 *            big any n
 * Helpers:   pow_int
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow_int.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    seq_t p2 = pow_int((seq_t)2, n);
    seq_t p4 = pow_int((seq_t)4, n);
    seq_t p5 = pow_int((seq_t)5, n);
    seq_t p6 = pow_int((seq_t)6, n);
    return p6 + p2 + (seq_t)1 - p5 - p4;
}
