/*
 * A215602  a(n) = L(n)*L(n+1), where L = A000032 (Lucas numbers).
 * https://oeis.org/A215602
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = L(n) * L(n+1) where L(n) are the Lucas numbers.
 *            Implementation uses fast doubling Fibonacci to obtain Lucas
 *            values
 * Range:     u8 n <= 5, u16 n <= 11, u32 n <= 22, u64 n <= 45, u128 n <= 91,
 *            big any n
 * Helpers:   fib_pair
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/fib_pair.h"

seq_t a(idx_t n)
{
    seq_t fn, fn1;                 /* F(n), F(n+1)          */
    fib_pair(n, &fn, &fn1);

    seq_t ln = fn1 * (seq_t)2 - fn; /* L(n) = 2*F(n+1) - F(n) */

    seq_t fnp1, fnp2;              /* F(n+1), F(n+2)        */
    fib_pair(n + 1, &fnp1, &fnp2);

    seq_t ln1 = fnp2 * (seq_t)2 - fnp1; /* L(n+1) = 2*F(n+2) - F(n+1) */

    return ln * ln1;               /* a(n) = L(n) * L(n+1)  */
}
