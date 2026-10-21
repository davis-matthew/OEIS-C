/*
 * A077537  Sum of next F(n) Fibonacci numbers, where F(n) = n-th Fibonacci
 *          number.
 * https://oeis.org/A077537
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = F_{F(n+2)+1} - F_{F(n+1)+1} * where F(k) is the k-th
 *            Fibonacci number (F(0)=0, F(1)=1). * This follows from a(n) =
 *            sum_{k=0}^{F(n)-1} F_{F(n+1)+k} * and the identity
 *            sum_{i=0}^{m-1} F_{p+i} = F_{p+m+1} - F_{p+1}
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 7, u64 n <= 9, u128 n <= 10,
 *            big any n
 * Helpers:   fib
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/fib.h"

seq_t a(idx_t n)
{
    /* Fibonacci numbers at indices n, n+1, n+2 */
    seq_t fn  = fib(n);      /* F(n)   */
    seq_t fn1 = fib(n + 1);  /* F(n+1) */
    seq_t fn2 = fib(n + 2);  /* F(n+2) */

    /* Indices for the two Fibonacci numbers we need to subtract */
    idx_t idx1 = (idx_t)(fn2 + (seq_t)1); /* F(n+2)+1 */
    idx_t idx2 = (idx_t)(fn1 + (seq_t)1); /* F(n+1)+1 */

    /* Compute the required Fibonacci numbers */
    seq_t term1 = fib(idx1); /* F_{F(n+2)+1} */
    seq_t term2 = fib(idx2); /* F_{F(n+1)+1} */

    /* Return the difference */
    return term1 - term2;
}
