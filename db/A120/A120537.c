/*
 * A120537  Sum of all matrix elements of n X n matrix M[i,j] = Lucas[i+j-1],
 *          (i,j = 1..n), where Lucas[n] = A000032[n] = Fibonacci[n-1] +
 *          Fibonacci[n+1].
 * https://oeis.org/A120537
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Lucas(2*n+3) - 2*Lucas(n+3) + 4, where Lucas(k) =
 *            Fib(k-1) + Fib(k+1) (Fib(0)=0, Fib(1)=1)
 * Range:     u8 n <= 4, u16 n <= 10, u32 n <= 21, u64 n <= 44, u128 n <= 90,
 *            big any n
 * Helpers:   fib
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/fib.h"

seq_t a(idx_t n)
{
    /* Compute Lucas(2*n+3) */
    idx_t m1 = 2*n + 2;   /* 2*n+2  (for Fib(2*n+2)) */
    idx_t m2 = 2*n + 4;   /* 2*n+4  (for Fib(2*n+4)) */
    seq_t luc2n3 = fib(m1) + fib(m2);

    /* Compute Lucas(n+3) */
    idx_t p1 = n + 2;     /* n+2   (for Fib(n+2)) */
    idx_t p2 = n + 4;     /* n+4   (for Fib(n+4)) */
    seq_t lucn3 = fib(p1) + fib(p2);

    /* a(n) = luc2n3 - 2*lucn3 + 4 */
    seq_t res = luc2n3;
    seq_t twice = lucn3 + lucn3;   /* 2 * lucn3 */
    res -= twice;
    res += 4;
    return res;
}
