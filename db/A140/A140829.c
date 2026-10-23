/*
 * A140829  a(0)=1; for n >= 1, a(n) = ceiling(Fibonacci(n)/a(n-1)).
 * https://oeis.org/A140829
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1; for n ≥ 1, a(n) = ceil(Fibonacci(n) / a(n-1))
 * Range:     u8 n <= 13, u16 n <= 24, u32 n <= 47, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for already computed terms */
    static seq_t *cache   = NULL;   /* a-values */
    static size_t  cap    = 0;      /* allocated size */
    static size_t  len    = 0;      /* number of terms stored       */
    /* static state for Fibonacci sequence */
    static seq_t  fib_prev2 = 0;     /* F(0) */
    static seq_t  fib_prev1 = 1;     /* F(1) */
    static idx_t  fib_curr  = 1;     /* index of fib_prev1 (currently 1) */

    /* Helper to ensure cache is long enough and fill missing entries */
    if (len == 0) {
        /* initialize first value a(0) = 1 */
        if (cap == 0) {
            cap = 1;
            cache = (seq_t *)malloc(cap * sizeof(seq_t));
            if (!cache) abort();
        }
        cache[0] = 1;
        len = 1;
    }

    /* Expand cache if necessary */
    if (len <= n) {
        size_t new_cap = (cap > 0) ? cap : 1;
        while (new_cap <= n) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();
        cache = tmp;
        cap = new_cap;
        /* Compute missing terms up to n */
        for (idx_t i = len; i <= n; ++i) {
            seq_t fib_i;
            if (i == 0) {
                fib_i = 0;
            } else if (i == 1) {
                fib_i = 1;
            } else {
                /* generate Fibonacci numbers up to i */
                while (fib_curr < i) {
                    seq_t sum = fib_prev1 + fib_prev2;
                    fib_prev2 = fib_prev1;
                    fib_prev1 = sum;
                    ++fib_curr;
                }
                fib_i = fib_prev1; /* now fib_prev1 == F(i) */
            }

            seq_t prev_a = cache[i - 1];
            /* Ceil division: (fib_i + prev_a - 1) / prev_a   */
            seq_t div = fib_i / prev_a;
            if (fib_i % prev_a != 0) ++div;
            cache[i] = div;
        }
        len = n + 1;
    }

    return cache[n];
}
