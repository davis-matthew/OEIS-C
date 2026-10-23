/*
 * A025169  a(n) = 2*Fibonacci(2*n+2).
 * https://oeis.org/A025169
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*Fibonacci(2*n+2) Initial terms: a(0)=2, a(1)=6
 *            Implementation uses a cache of Fibonacci numbers to avoid
 *            repeated recomputation and to stay within seq_t bounds
 * Range:     u8 n <= 4, u16 n <= 10, u32 n <= 22, u64 n <= 45, u128 n <= 91,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Cache of Fibonacci numbers.  fib_cache[0] = 0, fib_cache[1] = 1. */
    static seq_t *fib_cache = NULL;
    static size_t fib_len = 0;

    /* Initialise the Fibonacci cache on the first call. */
    if (fib_len == 0) {
        fib_len = 2;
        fib_cache = (seq_t *)malloc(fib_len * sizeof(seq_t));
        if (!fib_cache) abort();
        fib_cache[0] = (seq_t)0;
        fib_cache[1] = (seq_t)1;
    }

    /* Desired Fibonacci index: 2*n + 2. */
    size_t target = (size_t)(2 * n + 2);

    /* Grow the Fibonacci cache to include the desired index, if necessary. */
    if (target >= fib_len) {
        size_t new_len = target + 1;
        seq_t *tmp = (seq_t *)realloc(fib_cache, new_len * sizeof(seq_t));
        if (!tmp) abort();
        fib_cache = tmp;
        for (size_t i = fib_len; i <= target; ++i)
            fib_cache[i] = fib_cache[i - 1] + fib_cache[i - 2];
        fib_len = new_len;
    }

    /* a(n) = 2 * Fib(2*n + 2). */
    seq_t res = fib_cache[target] + fib_cache[target];  /* multiply by 2 */
    return res;
}
