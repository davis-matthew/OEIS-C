/*
 * A097478  Atomic numbers of halogens in the periodic table.
 * https://oeis.org/A097478
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1) = 9; a(n) = a(n‑1) + 2 * (ceil(n/2)+1)^2 for n ≥ 2
 * Range:     u8 n <= 6, u16 n <= 6, u32 n <= 6, u64 n <= 6, u128 n <= 6,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* cached terms */
    static seq_t *cache = NULL;
    static idx_t cached = 0;           /* number of terms currently cached */

    /* initialize cache with the first term */
    if (cached == 0) {
        cache = (seq_t *)malloc(sizeof(seq_t));
        if (!cache) return (seq_t)0;  /* fallback, should not happen in tests */
        cache[0] = (seq_t)9;         /* a(1) */
        cached = 1;
    }

    /* if requested term already cached, return it */
    if (n <= cached)
        return cache[n - 1];

    /* grow cache to accommodate up to n terms */
    seq_t *new_cache = (seq_t *)realloc(cache, sizeof(seq_t) * n);
    if (!new_cache) return (seq_t)0;  /* fallback */
    cache = new_cache;

    /* compute missing terms */
    for (idx_t i = cached; i < n; ++i) {      /* i is 1‑based index of term to compute */
        idx_t idx = i + 1;                    /* actual n for recurrence */
        seq_t x = (seq_t)((idx + 1) / 2 + 1);  /* ceil(idx/2)+1 */
        seq_t inc = 2 * x * x;                /* increment */
        cache[i] = cache[i - 1] + inc;        /* a(idx) = a(idx-1)+inc */
    }
    cached = n;

    return cache[n - 1];
}
