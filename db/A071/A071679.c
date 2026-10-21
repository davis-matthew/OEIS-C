/*
 * A071679  Least k such that the maximum number of elements among the
 *          continued fractions for k/1, k/2, k/3, k/4, ..., k/k equals n.
 * https://oeis.org/A071679
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(1) = 1 a(n) = Fib(n+2) for n > 1 where Fib(0)=0, Fib(1)=1
 * Range:     u8 n <= 11, u16 n <= 22, u32 n <= 45, u64 n <= 91, u128 n <= 184,
 *            big any n
 * Helpers:   fib
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/fib.h"

seq_t a(idx_t n)
{
    if (n <= 1)
        return (seq_t)1;
    return fib(n + 2);
}
