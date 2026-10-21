/*
 * A056585  Eighth power of Fibonacci numbers A000045.
 * https://oeis.org/A056585
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = F(n)^8, where F(n) is the Fibonacci number A000045
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 7, u64 n <= 13, u128 n <= 24,
 *            big any n
 * Helpers:   fib_pair, pow_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/fib_pair.h"
#include "../utils/pow_seq.h"

seq_t a(idx_t n)
{
    seq_t fn, fn1;
    fib_pair(n, &fn, &fn1);   /* fn = F(n), fn1 = F(n+1) */
    return pow_seq(fn, (idx_t)8);
}
