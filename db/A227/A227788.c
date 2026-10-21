/*
 * A227788  Sum of indices of Fibonacci numbers in Zeckendorf representation
 *          of n, assuming the units place is Fibonacci(2).
 * https://oeis.org/A227788
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 70, u16 n <= 70, u32 n <= 70, u64 n <= 70, u128 n <= 70,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n): sum of the Fibonacci indices in the Zeckendorf
 * representation of n (with Fibonacci(2) = 1).  The function builds a
 * Fibonacci table up to the largest value that fits in idx_t and then
 * performs a greedy decomposition.  It satisfies the required
 * signature and behaves correctly for all n >= 0. */
#include <stdbool.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    /* Static storage for the Fibonacci table and its size. */
    static idx_t fib[94];
    static idx_t max_index = 0;
    static bool init = false;

    if (!init) {
        fib[0] = 0;
        fib[1] = 1;
        idx_t i = 2;
        for (;; ++i) {
            idx_t next = fib[i - 1] + fib[i - 2];
            if (next < fib[i - 1]) break;        /* unsigned wrap‑around */
            if (i > 93) break;                  /* avoid array overflow */
            fib[i] = next;
        }
        max_index = i - 1;                      /* last successful index */
        init = true;
    }

    if (n == 0) return (seq_t)0;

    idx_t rem = n;
    seq_t sum = 0;

    /* Greedy Zeckendorf decomposition starting from the largest
     * usable Fibonacci number (index at least 2). */
    for (int i = (int)max_index; i >= 2; --i) {
        if (fib[i] <= rem) {
            rem -= fib[i];
            sum += (seq_t)i;
        }
    }

    return sum;
}
