/*
 * A048757  Sum_{i=0..2n} (C(2n,i) mod 2)*Fibonacci(i+2) = Sum_{i=0..n}
 *          (C(n,i) mod 2)*Fibonacci(2i+2).
 * https://oeis.org/A048757
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = Sum_{i=0..n} (C(n,i) mod 2) * Fib(2i+2) (using Lucas'
 *            theorem, C(n,i) is odd iff (i & (n-i)) == 0)
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 22, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include <stddef.h>

/* Cached Fibonacci numbers up to the highest index requested. */
static seq_t *fib_cache = NULL;     /* array of Fibonacci numbers [0..k] */
static idx_t  fib_cache_size = 0;   /* highest index currently cached */

/* Expand the fibonacci cache so that indices up to k are available. */
static void ensure_fib_cache(idx_t k)
{
    /* If the cache is empty, allocate it and compute up to k. */
    if (fib_cache == NULL) {
        size_t new_len = (size_t)(k + 1);
        fib_cache = (seq_t *)malloc(sizeof(seq_t) * new_len);
        if (!fib_cache) return;          /* silently fail on bad alloc */
        fib_cache[0] = 0;
        fib_cache[1] = 1;
        fib_cache_size = 1;
        /* Compute remaining Fibonacci numbers if needed. */
        if (k > 1) {
            for (idx_t i = 2; i <= k; ++i)
                fib_cache[i] = fib_cache[i-1] + fib_cache[i-2];
            fib_cache_size = k;
        }
    }
    /* If the cache is too small, enlarge it and compute the missing entries. */
    else if (k > fib_cache_size) {
        size_t new_len = (size_t)(k + 1);
        seq_t *tmp = (seq_t *)realloc(fib_cache, sizeof(seq_t) * new_len);
        if (!tmp) return;
        fib_cache = tmp;
        for (idx_t i = fib_cache_size + 1; i <= k; ++i)
            fib_cache[i] = fib_cache[i-1] + fib_cache[i-2];
        fib_cache_size = k;
    }
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    /* Need Fibonacci numbers up to 2n+2. */
    idx_t fib_limit = 2 * n + 2;
    ensure_fib_cache(fib_limit);

    seq_t sum = 0;
    for (idx_t i = 0; i <= n; ++i) {
        /* C(n,i) mod 2 is 1 iff i & (n-i) == 0. */
        if ((i & (n - i)) == 0) {
            sum += fib_cache[2 * i + 2];
        }
    }
    return sum;
}
