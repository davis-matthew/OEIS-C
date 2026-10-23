/*
 * A187107  Number of nontrivial compositions of differential operations and
 *          directional derivative of the n-th order on the space R^9.
 * https://oeis.org/A187107
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = A000045(n) + 7
 * Range:     u8 n <= 13, u16 n <= 24, u32 n <= 38, u64 n <= 38, u128 n <= 38,
 *            big any n
 * Helpers:   fib
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A000045(n) is the nth Fibonacci number with F(1)=1, F(2)=1 */
#include <stddef.h>
#include "../utils/fib.h"

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;               /* behavior below offset is unspecified */
    return fib(n) + (seq_t)7;
}
