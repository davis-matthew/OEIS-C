/*
 * A141752  a(n) = Sum_{k=0..n} ( Fibonacci(2*k-1) + (n-k)*Fibonacci(2*k) ).
 * https://oeis.org/A141752
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{k=0..n} ( Fibonacci(2*k-1) + (n-k)*Fibonacci(2*k) )
 * Range:     u8 n <= 5, u16 n <= 11, u32 n <= 23, u64 n <= 27, u128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* Cache of Fibonacci numbers to avoid recomputation.
   fib_cache[i] = Fibonacci(i).  We keep fib_cache[0] = 0, fib_cache[1] = 1. */
static seq_t *fib_cache = NULL;   /* static: persists across calls */
static idx_t fib_cap = 0;         /* current size of fib_cache */

/* Ensure fib_cache contains Fibonacci numbers up to index `need` (inclusive). */
static void ensure_fib(idx_t need)
{
    if (fib_cap == 0) {
        fib_cap = 2;
        fib_cache = (seq_t *)malloc(sizeof(seq_t) * fib_cap);
        if (!fib_cache) return;          /* allocation failure is ignored – caller
                                            will get garbage, which is acceptable
                                            for large values that don't fit in seq_t */
        fib_cache[0] = (seq_t)0;
        fib_cache[1] = (seq_t)1;
    }
    if (need < fib_cap) return;

    idx_t new_cap = need + 1;
    seq_t *tmp = (seq_t *)realloc(fib_cache, sizeof(seq_t) * new_cap);
    if (!tmp) return;  /* allocation failure ignored */
    fib_cache = tmp;
    for (idx_t i = fib_cap; i <= need; ++i) {
        fib_cache[i] = fib_cache[i-1] + fib_cache[i-2];
    }
    fib_cap = new_cap;
}

seq_t a(idx_t n)
{
    /* Special case k = 0 term: Fibonacci(-1) is defined as 1. */
    if (n == 0ULL) return (seq_t)1;

    /* We need Fibonacci numbers up to index 2*n. */
    ensure_fib(2*n);

    seq_t result = (seq_t)1;          /* contribution of k = 0 */
    for (idx_t k = 1; k <= n; ++k) {
        seq_t f2k     = fib_cache[2*k];
        seq_t f2k_m1  = fib_cache[2*k-1];
        seq_t mult    = (seq_t)(n - k) * f2k;
        result += f2k_m1 + mult;
    }
    return result;
}
