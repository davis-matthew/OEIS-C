/*
 * A227789  Sum of indices of Fibonacci numbers in Zeckendorf representation
 *          of n, assuming that the units place is Fibonacci(1).
 * https://oeis.org/A227789
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum of indices of Fibonacci numbers in the Zeckendorf
 *            representation of n, where Fibonacci(1) = 1
 * Range:     u8 n <= 70, u16 n <= 70, u32 n <= 70, u64 n <= 70, u128 n <= 70,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

/* Zeckendorf decomposition with Fibonacci numbers F(1)=1, F(2)=1, F(3)=2, …  */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;

    static idx_t fib[95];
    static size_t fib_last = 0;
    static bool initialized = false;

    /* Build or extend the Fibonacci table up to the largest term <= n. */
    if (!initialized || n > fib[fib_last]) {
        fib[1] = 1;
        fib[2] = 1;
        size_t lim = 2;
        while (true) {
            if (lim + 1 >= 95)            /* safety margin: F(94) > 2^64-1 */
                break;
            unsigned __int128 next = (unsigned __int128)fib[lim] + fib[lim - 1];
            if (next > n)
                break;
            fib[lim + 1] = (idx_t)next;
            lim++;
        }
        fib_last = lim;
        initialized = true;
    }

    idx_t rem = n;
    seq_t sum = 0;
    idx_t i = fib_last;

    /* Greedy Zeckendorf algorithm.  If F(2) would be used, replace
       it by F(1) (index 1) and subtract F(1). */
    while (rem > 0 && i > 0) {
        if (fib[i] <= rem) {
            idx_t idx = i;
            if (idx == 2)          /* treat the second 1 as Fibonacci(1) */
                idx = 1;
            sum += (seq_t)idx;
            rem -= fib[idx];
            i -= 2;                /* skip the next Fibonacci number to avoid
                                     consecutive indices */
        } else {
            i--;
        }
    }

    return sum;
}
