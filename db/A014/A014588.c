/*
 * A014588  Nim function for Take-a-Fibonacci-Game (a subtraction game).
 * https://oeis.org/A014588
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0)=0; for n>0, a(n)=mex{ a(n-f) | f∈{1,2,3,5,8,…} ≤ n } */
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* static cache for computed a(·) values */
    static seq_t *cache = NULL;
    static size_t cache_alloc = 0;   /* total allocated size */
    static size_t cache_end   = 0;   /* first index not yet computed */

    /* static cache for Fibonacci numbers */
    static uint64_t *fib = NULL;
    static size_t fib_len = 0;

    /* initialise fibonacci cache */
    if (fib_len == 0) {
        fib_len = 2;
        fib = (uint64_t*)malloc(fib_len * sizeof(uint64_t));
        if (!fib) exit(1);
        fib[0] = 1;
        fib[1] = 2;
    }
    while (fib_len < 2 || fib[fib_len - 1] < n) {
        uint64_t next = fib[fib_len - 2] + fib[fib_len - 1];
        uint64_t *tmp = (uint64_t*)realloc(fib, (fib_len + 1) * sizeof(uint64_t));
        if (!tmp) exit(1);
        fib = tmp;
        fib[fib_len++] = next;
    }

    /* ensure cache large enough for index n */
    if (cache_alloc < (size_t)n + 1) {
        cache = (seq_t*)realloc(cache, ((size_t)n + 1) * sizeof(seq_t));
        if (!cache) exit(1);
        cache_alloc = (size_t)n + 1;
    }

    /* compute any missing values up to n */
    static bool seen[256];
    if (cache_end <= n) {
        for (idx_t k = cache_end; k <= n; ++k) {
            memset(seen, 0, sizeof(seen));
            for (size_t i = 0; i < fib_len && fib[i] <= k; ++i) {
                seq_t val = cache[k - fib[i]];
                size_t idx = (size_t)val;
                if (idx < 256) seen[idx] = true;
            }
            size_t mex = 0;
            while (mex < 256 && seen[mex]) ++mex;
            cache[k] = (seq_t)mex;
        }
        cache_end = (size_t)n + 1;
    }

    return cache[n];
}
