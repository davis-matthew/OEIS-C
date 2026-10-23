/*
 * A171918  Van Eck sequence (cf. A181391) starting with a(1) = 8.
 * https://oeis.org/A171918
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1) = 8; for n ≥ 1, a(n+1) = 0 if a(n) has not appeared before,
 *            otherwise a(n+1) = n – (last position of a(n) before n)
 * Range:     u8 n <= 92, u16 n <= 92, u32 n <= 92, u64 n <= 92, u128 n <= 92,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  The implementation keeps a cache of the sequence terms and a table that
    maps each value to the last position where it occurred.  The table
    is expanded on demand.  All indices and arithmetic are performed in
    64‑bit unsigned integers; the value type seq_t may be fixed‑width or
    arbitrary‑precision as supplied by the build.  */

#include <stddef.h>
#include <stdlib.h>
#include <string.h>

seq_t a(idx_t n)
{
    /* static state */
    static seq_t  *cache      = NULL;   /* computed a(k) for k=1..cache_len */
    static size_t  cache_cap  = 0;
    static size_t  cache_len  = 0;

    static idx_t  *last       = NULL;   /* last occurrence of each value
                                           (1‑based index) */
    static size_t  last_cap   = 0;

    if (n == 0)
        return (seq_t)0;                 /* undefined position, return 0 */

    /* initialise first term lazily */
    if (cache_len == 0) {
        cache = (seq_t*)malloc(sizeof(seq_t));
        if (!cache) exit(EXIT_FAILURE);
        cache[0] = (seq_t)8;             /* a(1) = 8 */
        cache_cap = 1;
        cache_len = 1;
    }

    /* grow cache and last array up to required n */
    if ((size_t)n > cache_len) {
        /* enlarge cache if necessary */
        if (cache_cap < (size_t)n) {
            seq_t *tmp = (seq_t*)realloc(cache, (size_t)n * sizeof(seq_t));
            if (!tmp) exit(EXIT_FAILURE);
            cache = tmp;
            cache_cap = (size_t)n;
        }

        /* generate terms cache_len+1 .. n */
        for (idx_t idx = (idx_t)(cache_len + 1); idx <= n; ++idx) {
            /* previous value a(idx‑1) */
            seq_t prev_val = cache[idx - 2];

            /* map seq_t value to index of 'last' array */
            idx_t val_index = (idx_t)prev_val;   /* values are always < idx */

            /* ensure 'last' can index this value */
            if ((size_t)val_index >= last_cap) {
                size_t new_cap = (size_t)val_index + 1;
                idx_t *tmp = (idx_t*)realloc(last, new_cap * sizeof(idx_t));
                if (!tmp) exit(EXIT_FAILURE);
                /* initialise newly added slots to 0 */
                if (new_cap > last_cap)
                    memset(tmp + last_cap, 0, (new_cap - last_cap) * sizeof(idx_t));
                last = tmp;
                last_cap = new_cap;
            }

            /* previous occurrence of prev_val before position idx‑1 */
            idx_t last_occ = last[val_index];   /* 0 if none */

            /* compute a(idx) */
            seq_t next_val;
            if (last_occ == 0)                /* never seen before */
                next_val = (seq_t)0;
            else
                next_val = (seq_t)((idx - 1) - last_occ);

            /* store new term */
            cache[idx - 1] = next_val;

            /* update last occurrence table: prev_val now appears at idx‑1 */
            last[val_index] = idx - 1;
        }
        cache_len = n;
    }

    return cache[n - 1];
}
