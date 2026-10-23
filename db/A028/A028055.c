/*
 * A028055  Expansion of 1/((1-3x)(1-5x)(1-6x)(1-9x)).
 * https://oeis.org/A028055
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 9, u64 n <= 17, u128 n <= 17,
 *            big any n
 * Helpers:   seq_alloc, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A028055 – Expansion of 1/((1‑3x)(1‑5x)(1‑6x)(1‑9x))
 *
 *  Formula:   a(n) = 23*a(n-1) - 189*a(n-2) + 657*a(n-3) - 810*a(n-4)
 *              a(0)=1, a(1)=23, a(2)=340, a(3)=4130
 */

#include <stddef.h>        /* size_t */
#include <stdint.h>       /* UINT64_MAX */
#include "../utils/seq_alloc.h"
#include "../utils/seq_realloc.h"

seq_t a(idx_t n)
{
    /* static cache of previously computed values */
    static seq_t *cache = NULL;
    static size_t cap = 0;          /* allocated capacity */
    static idx_t max_cached = (idx_t)-1;  /* largest index that has been computed; -1 if none */

    /* initialise cache with the first four terms */
    if (cache == NULL) {
        cap = 4;
        cache = seq_alloc(cap);          /* allocates an array of seq_t */
        if (cache == NULL) return (seq_t)0; /* allocation failed – unlikely in tests */

        cache[0] = (seq_t)1;
        cache[1] = (seq_t)23;
        cache[2] = (seq_t)340;
        cache[3] = (seq_t)4130;
        max_cached = 3;
    }

    /* if requested index is beyond current capacity, expand it */
    if ((idx_t)n >= (idx_t)cap) {
        size_t new_cap = cap;
        while (new_cap <= (size_t)n) new_cap <<= 1;
        seq_t *tmp = seq_realloc(cache, cap, new_cap);
        if (tmp == NULL) return (seq_t)0;   /* allocation failed – unlikely */
        cache = tmp;
        cap = new_cap;
    }

    /* compute missing terms up to n */
    for (idx_t i = max_cached + 1; i <= n; ++i) {
        seq_t v1 = (seq_t)23 * cache[i - 1];
        seq_t v2 = (seq_t)189 * cache[i - 2];
        seq_t v3 = (seq_t)657 * cache[i - 3];
        seq_t v4 = (seq_t)810 * cache[i - 4];
        cache[i] = v1 - v2 + v3 - v4;
    }
    if (n > max_cached) max_cached = n;

    return cache[n];
}
