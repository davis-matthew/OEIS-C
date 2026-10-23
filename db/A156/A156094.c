/*
 * A156094  5 F(2n) (F(2n) - 1) + 1 where F(n) denotes the n-th Fibonacci
 *          number.
 * https://oeis.org/A156094
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 5*F(2n)*(F(2n) - 1) + 1
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 11, u64 n <= 20, u128 n <= 20,
 *            big any n
 * Helpers:   fib
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* where F(k) is the k‑th Fibonacci number, F(0)=0, F(1)=1 */

#include "../utils/fib.h"

seq_t a(idx_t n)
{
    /* Compute F(2n). */
    seq_t f2n = fib(n << 1);

    /* Work out 5*F(2n)*(F(2n)-1)+1. */
    seq_t tmp = f2n * (f2n - 1);  /* F(2n) * (F(2n)-1) */
    tmp = tmp * 5;                /* times 5 */
    tmp = tmp + 1;                /* plus 1 */

    return tmp;
}
