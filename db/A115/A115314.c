/*
 * A115314  a(n) = gcd(Lucas(n)+1, Fibonacci(n)-1).
 * https://oeis.org/A115314
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = gcd(Lucas(n)+1, Fibonacci(n)-1)
 * Range:     u8 n <= 12, u16 n <= 24, u32 n <= 46, u64 n <= 180,
 *            u128 n <= 192, big any n
 * Helpers:   fib_pair, gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/fib_pair.h"
#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    seq_t fn, fn1;
    /* fib_pair returns F(n)=fn, F(n+1)=fn1 */
    fib_pair(n, &fn, &fn1);

    /* Lucas(n) = F(n-1)+F(n+1) = 2*F(n+1) - F(n) */
    seq_t lucas = fn1;
    lucas += fn1;           /* 2*F(n+1) */
    lucas -= fn;            /* 2*F(n+1) - F(n) */
    lucas += (seq_t)1;      /* Lucas(n)+1 */

    seq_t fib_minus = fn;
    fib_minus -= (seq_t)1;  /* Fibonacci(n)-1 */

    return gcd(lucas, fib_minus);
}
