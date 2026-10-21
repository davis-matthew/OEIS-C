/*
 * A097040  a(n) = 2*sum(C(n,2k+1)*F(2k), k=0..floor((n-1)/2)), where F(n) are
 *          Fibonacci numbers A000045.
 * https://oeis.org/A097040
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = F(2*n-1) - F(n+1) (OEIS A097040)
 * Range:     u8 n <= 7, u16 n <= 12, u32 n <= 24, u64 n <= 47, u128 n <= 93,
 *            big any n
 * Helpers:   fib
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/fib.h"

seq_t a(idx_t n)
{
    /* The sequence is defined for n >= 1; for safety, return 0 for n == 0 */
    if (n == 0)
        return 0;

    seq_t fib_2n_minus1 = fib(2 * n - 1);  /* F(2n-1) */
    seq_t fib_n_plus1   = fib(n + 1);      /* F(n+1) */

    return fib_2n_minus1 - fib_n_plus1;
}
