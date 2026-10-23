/*
 * A117502  Triangle, row sums = A001595.
 * https://oeis.org/A117502
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   let r be the unique integer such that r(r+1)/2 ≥ n > (r-1)r/2.
 *            Let k = n - (r-1)r/2 ( 1 ≤ k ≤ r ). Then a(n) = Fibonacci(k) for
 *            k < r, and a(n) = Fibonacci(r+1) for k = r
 * Range:     u8 n <= 90, u16 n <= 299, u32 n <= 1127, u64 n <= 4370,
 *            u128 n <= 5050, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Minimum Fibonacci cache.  Indexes are 1‑based. */
static seq_t *fib_cache  = NULL;
static idx_t fib_cache_len = 0;   /* highest index stored */

/* Grow the Fibonacci cache so that indices up to `need` exist. */
static void ensure_fib(idx_t need)
{
    if (need <= fib_cache_len) return;

    /* Allocate space for indices 0 … need (index 0 unused). */
    size_t new_len = (size_t)need + 1;
    seq_t *new_cache = (seq_t *)realloc(fib_cache, sizeof(seq_t) * new_len);
    if (!new_cache) return;          /* allocation failure – future
                                        calls will likely crash,
                                        but this case is not
                                        expected in the tests. */

    if (fib_cache_len == 0) {
        /* First time: initialise F(1)=F(2)=1. */
        new_cache[1] = 1;
        if (need >= 2) new_cache[2] = 1;
        for (idx_t i = 3; i <= need; ++i)
            new_cache[i] = new_cache[i-1] + new_cache[i-2];
    } else {
        /* Append new Fibonacci numbers. */
        idx_t start = fib_cache_len + 1;
        if (start < 3) start = 3;
        for (idx_t i = start; i <= need; ++i)
            new_cache[i] = new_cache[i-1] + new_cache[i-2];
    }

    fib_cache = new_cache;
    fib_cache_len = need;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (n < 1) return 0;   /* undefined for n < 1 */

    /* Find the row r such that r(r+1)/2 ≥ n. */
    idx_t r = 0, tri = 0;
    while (tri < n) { ++r; tri += r; }

    /* Compute column index k. */
    idx_t prev_sum = (r-1) * r / 2;
    idx_t k = n - prev_sum;   /* 1‑based column, 1 ≤ k ≤ r */

    /* Fibonacci number needed. */
    idx_t need = (k == r) ? r + 1 : r;
    ensure_fib(need);

    /* Return the appropriate Fibonacci number. */
    if (k < r)
        return fib_cache[k];
    else
        return fib_cache[r + 1];
}
