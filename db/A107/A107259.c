/*
 * A107259  Number of ways to represent n as Fibonacci(i) + triangular(j),
 *          i,j>=0.
 * https://oeis.org/A107259
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 101, u16 n <= 101, u32 n <= 101, u64 n <= 101,
 *            u128 n <= 101, big any n
 * Helpers:   int_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: a(n) = number of representations of n as Fibonacci(i) + triangular(j), where i,j ≥ 0.
 * The function counts each distinct value of Fibonacci(i) only once, even though some values
 * (e.g. 1) occur twice in the Fibonacci sequence.
 */

#include "../utils/int_sqrt.h"

seq_t a(idx_t n)
{
    seq_t count = 0;
    /* Generate Fibonacci numbers in ascending order, skipping duplicate values. */
    seq_t fib_prev = 0;          /* F(0)      */
    seq_t fib_curr = 1;          /* F(1)      */
    seq_t last_fib = (seq_t)-1;  /* sentinel */

    while (fib_prev <= (seq_t)n) {
        /* Only count distinct Fibonacci values. */
        if (fib_prev != last_fib) {
            /* Remaining part that must be triangular. */
            idx_t rem = n - (idx_t)fib_prev;
            /* Check if rem = j(j+1)/2 for some integer j.  Solve 8*rem + 1 = (2j+1)^2 */
            idx_t t = 8 * rem + 1;
            idx_t r = int_sqrt(t);      /* floor(sqrt(t)) */
            if (r * r == t && (r & 1)) {
                /* r is odd => (r-1)/2 is integer j. */
                count++;
            }
        }

        last_fib = fib_prev;
        /* Compute next Fibonacci number.  F(k+2) = F(k+1) + F(k). */
        seq_t fib_next = fib_prev + fib_curr;
        fib_prev = fib_curr;
        fib_curr = fib_next;
    }

    return count;
}
