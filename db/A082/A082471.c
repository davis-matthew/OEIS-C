/*
 * A082471  a(1)=1, a(n) = Sum_{k=1..n-1} Fibonacci(k)*a(k).
 * https://oeis.org/A082471
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (F(n-1)+1)*a(n-1) for n>=3, with a(1)=1, a(2)=1
 * Range:     u8 n <= 6, u16 n <= 8, u32 n <= 11, u64 n <= 15, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

seq_t a(idx_t n) {
    if (n == 0) return 0;           // undefined, but safe
    if (n == 1) return 1;
    if (n == 2) return 1;

    seq_t a_val = 1;                 /* a(2) */
    seq_t fib_prev = 1;              /* F(1) */
    seq_t fib_curr = 1;              /* F(2) */

    for (idx_t i = 3; i <= n; ++i) {
        seq_t factor = fib_curr + 1; /* F(i-1) + 1 */
        a_val = a_val * factor;     /* a(i) = (F(i-1)+1)*a(i-1) */

        /* advance Fibonacci: compute F(i) */
        seq_t fib_next = fib_prev + fib_curr;
        fib_prev = fib_curr;
        fib_curr = fib_next;
    }

    return a_val;
}
