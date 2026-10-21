/*
 * A219641  a(n) = n minus (number of 1's in Zeckendorf expansion of n).
 * https://oeis.org/A219641
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n - A007895(n) (number of 1's in the Zeckendorf expansion
 *            of n)
 * Range:     u8 n <= 258, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   init_fibs, largest_fib_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>   /* for size_t */
#include "../utils/init_fibs.h"
#include "../utils/largest_fib_idx.h"

/* ------------------------------------------------------------------ */

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n == 0)          /* offset 0 */
        return (seq_t)0;

    /* Static cache of Fibonacci numbers.  The array is large enough for
       the maximum value that fits in idx_t (uint64_t). */
    static idx_t fibs[94];
    static size_t fib_len = 0;
    if (fib_len == 0)
        init_fibs(fibs, &fib_len);   /* one‑time initialization */

    idx_t remaining = n;
    seq_t ones = 0;                  /* count of 1's in Zeckendorf representation */

    while (remaining > 0) {
        /* Find the largest Fibonacci <= remaining */
        size_t idx = largest_fib_idx(fibs, fib_len, remaining);
        remaining -= fibs[idx];
        ones += (seq_t)1;          /* increase count of 1's */
    }

    return (seq_t)n - ones;
}
