/*
 * A011769  a(0) = 1, a(n+1) = 3 * a(n) - F(n)*(F(n) + 1), where F(n) =
 *          A000045(n) is n-th Fibonacci number.
 * https://oeis.org/A011769
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0) = 1, a(n+1) = 3*a(n) - F(n)*(F(n)+1) , where F(n) is the
 *            n‑th Fibonacci number (F(0)=0, F(1)=1)
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 21, u64 n <= 41, u128 n <= 81,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Returns the n-th term of sequence A011769 (0‑based). */
seq_t a(idx_t n)
{
    /* Static cache of already computed terms. */
    static seq_t *cache     = NULL;   /* array holding a[0] … a[cache_len] */
    static idx_t cache_len  = 0;      /* highest index filled in cache */
    static idx_t cache_cap  = 0;      /* allocated size of the cache array */

    /* Fibonacci numbers corresponding to the current cache_len:
       fib_prev = F(cache_len),  fib_next = F(cache_len+1). */
    static seq_t fib_prev  = 0;       /* F(0) */
    static seq_t fib_next  = 1;       /* F(1) */

    /* Initialisation: create the cache for the first time. */
    if (cache == NULL) {
        cache_cap = 1;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) return 0;        /* allocation failure */
        cache[0] = 1;                /* a(0) = 1 */
        cache_len = 0;
        fib_prev = 0;                /* F(0) */
        fib_next = 1;                /* F(1) */
    }

    /* Extend the cache up to the requested index n. */
    if (n > cache_len) {
        /* Ensure enough space in the cache. */
        if (n + 1 > cache_cap) {
            idx_t new_cap = n + 1;
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp) return 0;        /* allocation failure */
            cache = tmp;
            cache_cap = new_cap;
        }

        /* Compute missing terms iteratively. */
        for (idx_t i = cache_len; i < n; ++i) {
            /* prod = F(i) * (F(i)+1) */
            seq_t prod = fib_prev * (fib_prev + 1);

            /* a(i+1) = 3*a(i) - prod */
            cache[i + 1] = cache[i] * 3 - prod;

            /* Advance Fibonacci numbers to F(i+1) and F(i+2). */
            seq_t new_fib_prev = fib_next;             /* = F(i+1)   */
            seq_t new_fib_next = fib_prev + fib_next;  /* = F(i+2)   */
            fib_prev = new_fib_prev;
            fib_next = new_fib_next;
        }
        cache_len = n;
    }

    return cache[n];
}
