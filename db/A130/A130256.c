/*
 * A130256  Minimal index k of an odd Fibonacci number A001519 such that
 *          A001519(k) = Fibonacci(2*k-1) >= n (the 'upper' odd Fibonacci
 *          Inverse).
 * https://oeis.org/A130256
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = minimal k such that Fibonacci(2*k-1) ≥ n * (upper odd
 *            Fibonacci inverse)
 * Range:     u8 n <= 98, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   fib
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/fib.h"

seq_t a(idx_t n)
{
    if (n <= 1)
        return 0;  /* a(0) = 0, a(1) = 0 */

    idx_t low = 1, high = 1;

    /* grow high until fib(2*high-1) ≥ n */
    while (fib(2 * high - 1) < (seq_t)n) {
        low = high;
        high <<= 1;          /* high = 2*high */
    }

    /* binary search between low+1 and high */
    while (low + 1 < high) {
        idx_t mid = low + ((high - low) >> 1);
        if (fib(2 * mid - 1) >= (seq_t)n)
            high = mid;
        else
            low = mid;
    }
    return (seq_t)high;
}
