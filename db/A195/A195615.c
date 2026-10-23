/*
 * A195615  Numerators b(n) of Pythagorean approximations b(n)/a(n) to 2.
 * https://oeis.org/A195615
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 17*a(n-1) + 17*a(n-2) - a(n-3), a(1)=15, a(2)=273,
 *            a(3)=4895
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 15, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Computes the n-th term of OEIS sequence A195615.
   n is 1‑based.  Calls may be in any order. */
seq_t a(idx_t n)
{
    /* 0‑based cache: cache[i] holds a(i) for i>=1.
       cache[0] is unused. */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;   /* number of allocated elements (cache_size = 0 when NULL) */

    /* Basic cases */
    if (n <= 3) {
        switch (n) {
            case 1: return (seq_t)15;
            case 2: return (seq_t)273;
            case 3: return (seq_t)4895;
        }
    }

    /* Allocate cache if first time */
    if (cache == NULL) {
        cache_size = 4;                      /* indices 1..3 will be used */
        cache = (seq_t *)malloc(cache_size * sizeof(seq_t));
        if (!cache) return 0;                /* allocation failure; return 0 as sentinel */
        cache[1] = (seq_t)15;
        cache[2] = (seq_t)273;
        cache[3] = (seq_t)4895;
    }

    /* Grow cache if needed */
    if (n >= cache_size) {
        idx_t new_size = n + 1;   /* one past maximum index needed */
        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!tmp) {               /* realloc failed, keep old cache */
            free(cache);
            cache = NULL;
            cache_size = 0;
            return 0;
        }
        cache = tmp;
        /* Compute and fill new entries */
        for (idx_t i = cache_size; i <= n; ++i) {
            /* recurrence: a(n) = 17*a(n-1) + 17*a(n-2) - a(n-3) */
            seq_t term = 17 * cache[i - 1] + 17 * cache[i - 2] - cache[i - 3];
            cache[i] = term;
        }
        cache_size = new_size;
    }
    return cache[n];
}
