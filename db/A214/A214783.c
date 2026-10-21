/*
 * A214783  a(n) = smallest k such that n divides Fibonacci(k-1)+3.
 * https://oeis.org/A214783
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = smallest k such that n divides Fibonacci(k-1)+3
 * Range:     u8 n <= 133, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

seq_t a(idx_t n)
{
    if (n == 0)            /* not defined for n=0, but guard anyway */
        return 0;

    seq_t mod = (seq_t)n;      /* modulo value, fits in seq_t */

    /* Fibonacci recurrence modulo mod */
    seq_t fib_prev = 0;                       /* F(0) */
    seq_t fib_curr = (mod == 0) ? 0 : 1 % mod;/* F(1) modulo mod */

    idx_t k = 1; /* current k corresponds to fib_prev = F(k-1) */

    /* Pisano period for modulus n is at most 6*n; we stop after that */
    idx_t limit = 6 * n + 3;

    for (; k <= limit; ++k) {
        /* check if n divides F(k-1)+3 */
        seq_t test = fib_prev + (seq_t)3;
        if (mod == 0 || test % mod == 0)
            return (seq_t)k;

        /* advance Fibonacci pair */
        seq_t next = fib_prev + fib_curr;
        if (mod != 0)
            next %= mod;      /* keep numbers small */

        fib_prev = fib_curr;
        fib_curr = next;
    }

    /* Fallback: should never be reached if Pisano bound holds */
    return (seq_t)k;
}
