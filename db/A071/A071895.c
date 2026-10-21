/*
 * A071895  CONTINUANT transform of Fibonacci number 1, 2, 3, 5, 8, ...
 * https://oeis.org/A071895
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-2) + F(n+1)*a(n-1); a(1)=1, a(2)=3
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 9, u64 n <= 13, u128 n <= 19,
 *            big any n
 * Helpers:   fib
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* N-th term of OEIS A071895 (continued fraction transform of Fibonacci numbers) */

#include <stdlib.h>        /* malloc, realloc */
#include "../utils/fib.h"  /* fib(idx_t n) */

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;   /* cache[i] holds a(i) */
    static idx_t  cache_max = 0;  /* highest index already computed */

    /* trivial case below offset */
    if (n == 0) return 0;

    /* initialise cache with the first two terms */
    if (cache == NULL) {
        cache = (seq_t *)malloc(3 * sizeof(seq_t));
        if (!cache) return 0;                /* out‑of‑memory (unlikely in tests) */
        cache[0] = 0;                        /* unused slot for 0‑index */
        cache[1] = 1;                        /* a(1) */
        cache[2] = 3;                        /* a(2) */
        cache_max = 2;
    }

    /* compute any missing terms up to n */
    if (n > cache_max) {
        size_t new_size = (size_t)(n + 1);
        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!tmp) return 0;                  /* out‑of‑memory */
        cache = tmp;

        for (idx_t i = cache_max + 1; i <= n; ++i) {
            seq_t fib_i1 = fib(i + 1);      /* F(i+1) */
            cache[i] = cache[i - 2] + fib_i1 * cache[i - 1];
        }
        cache_max = n;
    }

    return cache[n];
}
