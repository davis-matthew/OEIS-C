/*
 * A130235  Partial sums of the 'lower' Fibonacci Inverse A130233.
 * https://oeis.org/A130235
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 37, u16 n <= 4015, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation of A130235: partial sums of lower Fibonacci inverse.
   a(n) = ∑_{k=0}^n A130233(k) where A130233(k) = max m with Fib(m) ≤ k.
   The sum is obtained by accumulating m over the intervals [Fib(m),
   Fib(m+1)−1] that intersect [0,n]. */
#include <stdint.h>
#include <limits.h>

seq_t a(idx_t n)
{
    /* static table of Fibonacci numbers up to ULLONG_MAX */
    static uint64_t fib[95];
    static int fib_len = 0;
    if (!fib_len) {
        fib[0] = 0;
        fib[1] = 1;
        fib_len = 2;
        while (fib_len < 95) {                    /* guard against overflow */
            if (fib[fib_len - 1] > ULLONG_MAX - fib[fib_len - 2])
                break;
            fib[fib_len] = fib[fib_len - 1] + fib[fib_len - 2];
            ++fib_len;
        }
    }

    seq_t sum = 0;
    for (int m = 0; m < fib_len; ++m) {
        idx_t lower = fib[m];
        if (lower > n)
            break;
        idx_t upper;
        if (m + 1 < fib_len)
            upper = fib[m + 1] - 1;
        else
            upper = n;
        if (upper > n)
            upper = n;
        if (upper < lower)
            continue;
        idx_t count = upper - lower + 1;
        sum += (seq_t)m * (seq_t)count;
    }
    return sum;
}
