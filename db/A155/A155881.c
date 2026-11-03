/*
 * A155881  a(n) is the number of zeros needed to write the integers 1 through
 *          Fibonacci(n).
 * https://oeis.org/A155881
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = A061217(Fibonacci(n)) (zeros in 1..Fib(n))
 * Range:     u8 n <= 12, u16 n <= 24, u32 n <= 47, u64 n <= 92, u128 n <= 183,
 *            big any n
 * Validated: standard, pass, 58 terms through n = 4366 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

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

/* Count the number of digit zero occurrences in decimal
   representations of all integers from 1 to N (inclusive). */
static seq_t count_zeros_upto(seq_t N)
{
    seq_t total = 0;
    seq_t p = 1;           /* current position value (10^i) */

    while (p <= N) {
        seq_t div = N / p;                /* N divided by current position */
        seq_t high = div / 10;            /* digits left of current position */
        seq_t cur  = div % 10;            /* digit at current position */
        seq_t low  = N % p;               /* digits right of current position */

        if (high != 0) {
            seq_t term1 = (high - 1) * p; /* zeros from higher full blocks */

            seq_t term2;
            if (cur > 0) {
                term2 = p;                         /* full block of zeros */
            } else if (cur == 0) {
                term2 = low + 1;                    /* partial block */
            } else {                               /* cur < 0 unlikely */
                term2 = 0;
            }
            total += term1 + term2;
        }
        p *= 10;        /* move to next decimal position */
    }
    return total;
}

/* Main sequence function as required by the build system. */
seq_t a(idx_t n)
{
    seq_t fn = fib(n);                     /* Fibonacci(n) */
    return count_zeros_upto(fn);           /* zeros in 1..Fib(n) */
}
