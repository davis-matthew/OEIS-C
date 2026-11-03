/*
 * A219642  Number of steps to reach 0 starting with n and using the iterated
 *          process: x -> x - (number of 1's in Zeckendorf expansion of x).
 * https://oeis.org/A219642
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 0; for n > 0, a(n) = 1 + a(n - z(n)), where z(n) is the
 *            number of terms in the Zeckendorf expansion of n
 * Range:     u8 n <= 382, u16 n <= 10946, u32 n <= 10946, u64 n <= 10946,
 *            u128 n <= 10946, big any n
 * Validated: standard, pass, 86 terms through n = 9367 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* Count the number of terms in the Zeckendorf expansion of a non‑negative
   number n.  The Zeckendorf expansion uses the Fibonacci sequence
   F(1)=1, F(2)=2, F(k)=F(k-1)+F(k-2).  The greedy algorithm below
   repeatedly subtracts the largest Fibonacci number not exceeding n. */
static seq_t zeckendorf_count(seq_t n)
{
    seq_t count = 0;

    while (n > 0) {
        /* Find the largest Fibonacci number <= n. */
        seq_t fib_prev = 1;   /* F(1) */
        seq_t fib_curr = 2;   /* F(2) */
        seq_t prev, curr;

        while (fib_curr <= n) {
            prev  = fib_prev;
            curr  = fib_curr;
            fib_curr = prev + curr;   /* next Fibonacci */
            fib_prev = curr;          /* shift */
        }

        /* Subtract the largest Fibonacci number found. */
        n -= fib_prev;
        ++count;
    }

    return count;
}

/* OEIS sequence A219642. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;

    seq_t steps = 0;
    seq_t m = (seq_t)n;   /* work in seq_t to avoid conversion on loop */

    while (m > 0) {
        seq_t t = zeckendorf_count(m);
        m -= t;
        ++steps;
    }

    return steps;
}
