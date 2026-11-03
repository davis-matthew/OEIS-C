/*
 * A119282  Alternating sum of the first n Fibonacci numbers.
 * https://oeis.org/A119282
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = (-1)^n * F(n-1) - 1 Recurrence: a(0)=0, a(1)=-1, a(2)=0,
 *            a(n)=a(n-2)-a(n-1)-1 (n>=3)
 * Range:     i8 n <= 12, i16 n <= 24, i32 n <= 47, i64 n <= 93, i128 n <= 185,
 *            big any n
 * Validated: standard, pass, 67 terms through n = 958 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>   /* malloc, realloc */
#include "../utils/mem.h"

extern seq_t fib(idx_t n);   /* provided by utils/fib.h, not used directly here */

seq_t a(idx_t n)
{
    /* static cache of computed values */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;
    static int initialized = 0;
    /* base values */
    static const seq_t base[3] = {0, -1, 0};

    /* initialize cache once */
    if (!initialized) {
        cache_size = 3;
        cache = mem_new(seq_t, cache_size);
        if (!cache) return 0;                 /* out of memory – fall back to 0 */
        for (idx_t i = 0; i < cache_size; ++i)
            cache[i] = base[i];
        initialized = 1;
    }

    /* enlarge cache if necessary */
    if (n >= cache_size) {
        idx_t new_size = n + 1;                /* allocate exactly enough */
        seq_t *new_cache = mem_resize(seq_t, cache, new_size);
        if (!new_cache) return 0;              /* out of memory – return 0 */
        cache = new_cache;
        /* compute missing values using recurrence */
        for (idx_t i = cache_size; i <= n; ++i) {
            /* a(i) = a(i-2) - a(i-1) - 1 */
            cache[i] = cache[i - 2] - cache[i - 1] - 1;
        }
        cache_size = new_size;
    }

    return cache[n];
}
