/*
 * A062837  Numbers k such that k, 2*k+1, 3*k+1 are all squarefree.
 * https://oeis.org/A062837
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 31, u16 n <= 7805, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_squarefree
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A062837: numbers k such that k, 2*k+1, and 3*k+1 are all squarefree. */
/* Computation: iterate k from 1 upwards, test the three squarefree conditions, and memoise the results. */

#include <stdlib.h>         /* malloc, realloc, abort */
#include <stddef.h>         /* size_t */
#include <stdbool.h>        /* bool */
#include "../utils/is_squarefree.h"

seq_t a(idx_t n)
{
    if (n == 0)          /* offset is 1, so undefined for n=0 */
        return (seq_t)0;

    /* static cache of found terms */
    static seq_t *cache      = NULL;   /* array of k values */
    static size_t cache_len  = 0;      /* number of cached terms */
    static size_t cache_cap  = 0;      /* allocated capacity */
    static idx_t last_k      = 0;      /* last k examined */

    /* Ensure the cache holds at least n terms */
    while (cache_len < (size_t)n)
    {
        /* Next candidate k */
        last_k++;
        idx_t k      = last_k;
        idx_t k2     = k * 2 + 1;
        idx_t k3     = k * 3 + 1;

        /* Cast to seq_t for square‑free tests */
        seq_t sk  = (seq_t)k;
        seq_t sk2 = (seq_t)k2;
        seq_t sk3 = (seq_t)k3;

        if (is_squarefree(sk) &&
            is_squarefree(sk2) &&
            is_squarefree(sk3))
        {
            /* Grow the cache if necessary */
            if (cache_len == cache_cap)
            {
                size_t new_cap = (cache_cap == 0) ? 16 : cache_cap * 2;
                void *tmp = realloc(cache, new_cap * sizeof(seq_t));
                if (!tmp)
                    abort();                /* out of memory */
                cache = (seq_t *)tmp;
                cache_cap = new_cap;
            }
            cache[cache_len++] = sk;
        }
    }

    return cache[n - 1];
}
