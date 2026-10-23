/*
 * A122880  Catalan numbers minus odd-indexed Fibonacci numbers.
 * https://oeis.org/A122880
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A000108(n) - A001519(n) = catalan(n) - fib(2*n-1)
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 17, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Helpers:   catalan, fib
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/catalan.h"
#include "../utils/fib.h"

seq_t a(idx_t n)
{
    /* a(0) is undefined for the OEIS sequence (offset 1).  The
       function is only called with n >= 1.  For safety, return 0
       for n == 0. */
    if (n == 0)
        return (seq_t)0;

    seq_t cat = catalan(n);
    idx_t fib_index = (n << 1) - 1;      /* 2*n - 1 */
    seq_t fib_val = fib(fib_index);

    return cat - fib_val;
}
