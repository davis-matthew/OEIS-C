/*
 * A028125  Expansion of 1/((1-4x)(1-5x)(1-9x)(1-11x)).
 * https://oeis.org/A028125
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 29*a(n-1) - 299*a(n-2) + 1291*a(n-3) - 1980*a(n-4), n >=
 *            4; a(0)=1, a(1)=29, a(2)=542, a(3)=8338 (derived from
 *            1/((1-4x)(1-5x)(1-9x)(1-11x)))
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 8, u64 n <= 16, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *seq_cache = NULL;          /* cache array          */
static size_t cache_cap   = 0;           /* capacity of cache   */
static size_t cache_size  = 0;           /* number of values cached (next index to compute) */

static const seq_t init_seq[4] = {
    (seq_t)1,
    (seq_t)29,
    (seq_t)542,
    (seq_t)8338
};

seq_t a(idx_t n)
{
    size_t N = (size_t)n;   /* convert index to size_t for array indexing */

    /* trivial cases */
    if (N < 4)
        return init_seq[N];

    /* grow cache capacity if necessary */
    if (cache_cap <= N) {
        size_t new_cap = N + 1;
        seq_t *new_cache = (seq_t *)realloc(seq_cache, new_cap * sizeof(seq_t));
        if (!new_cache)
            abort();          /* out of memory – abort program */
        seq_cache = new_cache;
        cache_cap = new_cap;
    }

    /* initialise first four values if not already done */
    if (cache_size < 4) {
        for (size_t i = 0; i < 4; ++i)
            seq_cache[i] = init_seq[i];
    }

    /* compute missing terms up to N */
    for (size_t i = cache_size; i <= N; ++i) {
        if (i < 4)                 /* already set */
            continue;
        seq_cache[i] =
            (seq_t)((seq_t)29  * seq_cache[i-1] -
                    (seq_t)299 * seq_cache[i-2] +
                    (seq_t)1291 * seq_cache[i-3] -
                    (seq_t)1980 * seq_cache[i-4]);
    }

    cache_size = N + 1;             /* update number of cached entries   */
    return seq_cache[N];
}
