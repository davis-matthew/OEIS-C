/*
 * A147690  a(0)=1; thereafter a(n+1)=F(n+3)*a(n)+F(n+3) where F_n is
 *          Fibonacci's sequence 0,1,1,2,3,5,8, etc.
 * https://oeis.org/A147690
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=1; a(n+1)=F(n+3)*a(n)+F(n+3) where F is the Fibonacci
 *            sequence 0,1,1,2,3,…
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 8, u64 n <= 12, u128 n <= 16,
 *            big any n
 * Validated: partial, pass, 17 terms through n = 16 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/mem.h"

static seq_t fib(idx_t n) {
    if (n <= 2) return (seq_t)1;
    seq_t f1 = 1, f2 = 1, f3;
    for (idx_t i = 3; i <= n; ++i) {
        f3 = f1 + f2;
        f1 = f2;
        f2 = f3;
    }
    return f2;
}

seq_t a(idx_t n) {
    /* static cache to memoize values */
    static seq_t *cache = NULL;
    static size_t cap = 0;      /* allocated size of cache */
    static idx_t last = 0;      /* highest index computed so far */

    /* initialise cache on first call */
    if (cache == NULL) {
        cap = 1;
        cache = mem_new(seq_t, cap);
        if (!cache) return 0;   /* memory allocation failed */
        cache[0] = 1;           /* a(0) = 1 */
        last = 0;
    }

    /* grow cache if needed */
    if ((idx_t)cap <= n) {
        size_t newcap = (size_t)n + 1;
        seq_t *newcache = mem_new(seq_t, newcap);
        if (!newcache) return 0;
        for (size_t i = 0; i < cap; ++i) newcache[i] = cache[i];
        mem_free(cache);
        cache = newcache;
        cap = newcap;
    }

    /* compute missing values up to n */
    for (idx_t i = last + 1; i <= n; ++i) {
        seq_t fn = fib(i + 2);                  /* F(i+2) */
        cache[i] = fn * cache[i - 1] + fn;       /* a(i) = F(i+2)*a(i-1)+F(i+2) */
    }
    last = n;

    return cache[n];
}
