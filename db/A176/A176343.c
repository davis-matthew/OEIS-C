/*
 * A176343  a(n) = Fibonacci(n)*a(n-1) + 1, a(0) = 0.
 * https://oeis.org/A176343
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Fibonacci(n)*a(n-1) + 1, a(0) = 0
 * Range:     u8 n <= 5, u16 n <= 7, u32 n <= 10, u64 n <= 14, u128 n <= 20,
 *            big any n
 * Helpers:   fib
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Uses the provided fib(idx_t n) routine to get Fibonacci numbers. */

#include <stdlib.h>
#include "../utils/fib.h"   /* provides seq_t fib(idx_t n) */

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;  /* cache[0]..cache[cap-1] */
    static size_t cap   = 0;     /* allocated size */

    /* Ensure cache big enough for n+1 entries (0..n) */
    if (cap <= n) {
        size_t new_cap = n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return 0;            /* allocation failed – unlikely */
        cache = tmp;
        cap   = new_cap;
    }

    /* Handle the trivial case */
    if (n == 0) {
        cache[0] = 0;
        return 0;
    }

    /* Compute values from 1 up to n (in increasing order). */
    for (idx_t i = 1; i <= n; ++i) {
        /* fib(i) returns the i‑th Fibonacci number (F₀ = 0, F₁ = 1). */
        seq_t fi = fib(i);
        cache[i] = fi * cache[i - 1] + 1;
    }

    return cache[n];
}
