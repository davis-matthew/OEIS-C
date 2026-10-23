/*
 * A130260  Minimal index k of an even Fibonacci number A001906 such that
 *          A001906(k) = Fib(2k) >= n (the 'upper' even Fibonacci Inverse).
 * https://oeis.org/A130260
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = minimal k such that Fib(2k) >= n
 * Range:     u8 n <= 1305, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t target = (seq_t)n;          /* Convert the index to seq_t for comparison. */
    seq_t f0 = 0, f1 = 1;             /* Fibonacci numbers: f0 = F(i), f1 = F(i+1). */
    idx_t i = 0;                      /* Current Fibonacci index i. */

    while (1) {
        /* At even indices i = 2k, f0 equals Fib(2k). */
        if ((i & 1u) == 0) {
            if (f0 >= target) {
                return (seq_t)(i >> 1);   /* Return k = i/2. */
            }
        }

        /* Compute the next Fibonacci number. */
        seq_t next = f0 + f1;
        f0 = f1;
        f1 = next;
        ++i;
    }
}
