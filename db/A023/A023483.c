/*
 * A023483  a(n) = b(n) + d(n), where b(n) = (n-th Fibonacci number > 1) and
 *          d(n) = (n-th non-Fibonacci number).
 * https://oeis.org/A023483
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A000045(n+2) + A001690(n)
 * Range:     u8 n <= 11, u16 n <= 22, u32 n <= 36, u64 n <= 36, u128 n <= 36,
 *            big any n
 * Validated: partial, pass, 36 terms through n = 36 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the n‑th term of OEIS A023483.
   It uses the supplied fib routine for Fibonacci numbers and
   a simple gap‑counting algorithm for the n‑th non‑Fibonacci number. */

#include <stdbool.h>

static seq_t fib(idx_t n) {
    if (n <= 2) return (seq_t)1;
    seq_t f1 = 1, f2 = 1, f3;
    for (idx_t i = 3; i <= n; ++i) {
        f3 = f1 + f2;
        f1 = f2;
        f2 = f3;
    }
    return f2;
}

static seq_t
nth_non_fibonacci(idx_t n)
{
    /* Number of non‑Fibonacci numbers seen so far */
    seq_t count = 0;

    /* The first two Fibonacci numbers greater than 1
       (F2 = 2, F3 = 3) */
    seq_t prev = 2;   /* F2 */
    seq_t curr = 3;   /* F3 */

    while (1) {
        /* Numbers strictly between prev and curr */
        seq_t gap = curr - prev - (seq_t)1;

        /* If the desired index falls within this gap,
           compute the value directly. */
        if ((seq_t)n <= count + gap) {
            seq_t offset = (seq_t)n - count;
            return prev + offset;
        }

        /* Skip the entire gap */
        count += gap;

        /* Advance to the next Fibonacci number */
        seq_t next = prev + curr;
        prev = curr;
        curr = next;
    }
}

seq_t
a(idx_t n)
{
    /* n‑th Fibonacci number > 1 is A000045(n+2) */
    seq_t b = fib(n + (idx_t)2);

    /* n‑th non‑Fibonacci number is A001690(n) */
    seq_t d = nth_non_fibonacci(n);

    return b + d;
}
