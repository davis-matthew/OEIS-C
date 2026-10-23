/*
 * A022858  a(n) = [ a(n-1)/a(1) ] + [ a(n-2)/a(2) ] + ... + [ a(1)/a(n-1) ],
 *          for n >= 3.
 * https://oeis.org/A022858
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ⌊a(1)/a(n-1)⌋ + ⌊a(2)/a(n-2)⌋ + … + ⌊a(n-1)/a(1)⌋ (OEIS
 *            A022858) Initial values: a(1)=1, a(2)=1
 * Range:     u8 n <= 11, u16 n <= 21, u32 n <= 35, u64 n <= 35, u128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;          /* stored terms, 1‑based */
    static size_t cache_size = 0;        /* allocated size of cache */
    static idx_t max_computed = 0;       /* largest n for which cache[n] is valid */

    /* one‑time allocation and initialisation */
    if (!cache) {
        cache_size = 4;                  /* enough for indices 0…3 */
        cache = (seq_t *)malloc(cache_size * sizeof(seq_t));
        if (!cache) return 0;            /* allocation failed – silently give 0 */
        cache[0] = 0;                    /* unused */
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)1;
        max_computed = 2;
    }

    /* if the requested term is already known, return it */
    if (n <= max_computed)
        return cache[n];

    /* grow cache if necessary */
    if (cache_size <= (size_t)(n + 1)) {
        size_t new_size = cache_size ? cache_size * 2 : 4;
        while (new_size <= (size_t)(n + 1))
            new_size *= 2;
        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!tmp) return 0;              /* allocation failed */
        cache = tmp;
        cache_size = new_size;
    }

    /* compute missing terms in order */
    for (idx_t k = max_computed + 1; k <= n; ++k) {
        seq_t sum = 0;
        for (idx_t i = 1; i < k; ++i) {
            sum += cache[i] / cache[k - i];
        }
        cache[k] = sum;
    }
    max_computed = n;
    return cache[n];
}
