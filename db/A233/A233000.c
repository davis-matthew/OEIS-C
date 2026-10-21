/*
 * A233000  Let L(n) = Fibonacci(n-1)+Fibonacci(n+1) (cf. A000045, A000032);
 *          if n is even then a(n) = (L(n)+2)^2 otherwise a(n) = L(2*n)+2.
 * https://oeis.org/A233000
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   L(n) = fib(n-1)+fib(n+1); a(n) = (L(n)+2)^2 for even n,
 *            otherwise a(n) = L(2*n)+2
 * Range:     u8 n <= 5, u16 n <= 11, u32 n <= 23, u64 n <= 46, u128 n <= 92,
 *            big any n
 * Helpers:   fib
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/fib.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return 16;                 /* L(0)=fib(-1)+fib(1)=2, (2+2)^2 = 16 */

    if ((n & 1) == 0) {            /* even n */
        seq_t l = fib(n - 1) + fib(n + 1);   /* L(n) */
        seq_t v = l + 2;
        return v * v;                         /* (L(n)+2)^2 */
    } else {                       /* odd n */
        idx_t m = n << 1;          /* 2*n */
        seq_t l = fib(m - 1) + fib(m + 1);   /* L(2*n) */
        return l + 2;                  /* L(2*n)+2 */
    }
}
