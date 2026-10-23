/*
 * A019274  Number of recursive calls needed to compute the n-th Fibonacci
 *          number F(n), starting with F(1) = F(2) = 1.
 * https://oeis.org/A019274
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(n) = 2*F(n) - 2, where F(n) is the Fibonacci number with
 *            F(1)=F(2)=1
 * Range:     u8 n <= 11, u16 n <= 23, u32 n <= 46, u64 n <= 92, u128 n <= 184,
 *            big any n
 * Helpers:   fib
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/fib.h"

seq_t a(idx_t n) {
    /* Compute the nth Fibonacci number. */
    seq_t f = fib(n);
    /* a(n) = 2*F(n) - 2. */
    seq_t res = f * 2;
    res -= 2;
    return res;
}
