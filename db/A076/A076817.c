/*
 * A076817  Let u(1)=u(2)=1, u(n)=(3^floor(u(n-1)/2)+1)/u(n-2); then
 *          a(n)=10*u(n).
 * https://oeis.org/A076817
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 72, u32 n <= 72, u64 n <= 72, u128 n <= 72,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A076817
   a(1) = a(2) = 10
   a(n) = ((3^floor(a(n‑1)/20)+1) * 100) / a(n‑2)   for n ≥ 3
   (rounded to integer, all divisions are exact). */

#include <stdlib.h>

/* The build supplies the types seq_t and idx_t.  */
seq_t a(idx_t n)
{
    /* Static cache for previously computed terms. */
    static seq_t *cache = NULL;   /* cache[1] … cache[cache_len] */
    static idx_t cache_len = 0;   /* highest index stored so far */

    /* Lazy initialization – store first two terms. */
    if (cache_len == 0) {
        cache_len = 2;
        cache = (seq_t*)malloc((cache_len + 1) * sizeof(seq_t)); /* index 0 unused */
        cache[1] = (seq_t)10;
        cache[2] = (seq_t)10;
    }

    /* Grow the cache if the requested index is beyond the current size. */
    if (n > cache_len) {
        seq_t *new_cache = (seq_t*)realloc(cache, (n + 1) * sizeof(seq_t));
        if (!new_cache) {
            /* Allocation failed – in the constrained environment just abort. */
            abort();
        }
        cache = new_cache;
        for (idx_t i = cache_len + 1; i <= n; ++i) {
            seq_t prev = cache[i - 1];          /* a(n‑1) */
            seq_t prevprev = cache[i - 2];      /* a(n‑2) */

            /* k = floor(a(n‑1)/20) */
            seq_t k = prev / (seq_t)20;

            /* Compute 3^k */
            seq_t pow3 = (seq_t)1;
            for (seq_t j = 0; j < k; ++j)
                pow3 *= (seq_t)3;

            /* numerator = (pow3 + 1) * 100 */
            seq_t numer = (pow3 + (seq_t)1) * (seq_t)100;

            /* a(n) = numerator / a(n‑2) */
            cache[i] = numer / prevprev;
        }
        cache_len = n;
    }

    return cache[n];
}
