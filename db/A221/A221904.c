/*
 * A221904  a(n) = 9^n + 10^n.
 * https://oeis.org/A221904
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 9^n + 10^n; Recurrence: a(0)=2, a(1)=19, a(n)=19*a(n-1) -
 *            90*a(n-2)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 19, u128 n <= 38,
 *            big any n
 * Helpers:   seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/seq_pow.h"

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)2;
    seq_t pow9  = seq_pow((seq_t)9, n);
    seq_t pow10 = seq_pow((seq_t)10, n);
    return pow9 + pow10;
}
