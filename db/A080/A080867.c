/*
 * A080867  Dorey's period-90 sequence (periodic from a(1)=1 onward).
 * https://oeis.org/A080867
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = n mod 10 for n < 12; for n ≥ 12, a(n) = (a(n‑1) + a(i+1))
 *            mod 10, where i is the nearest earlier index that contains the
 *            value a(n‑1)
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Static cache for memoisation */
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;               /* number of elements stored (max index + 1) */

    /* For each decimal digit keep the last and second‑last occurrence indices. */
    static idx_t last_occ[10];
    static idx_t second_occ[10];
    static int  init = 0;

    if (!init) {
        for (int d = 0; d < 10; ++d) {
            last_occ[d]    = (idx_t)-1;
            second_occ[d]  = (idx_t)-1;
        }
        init = 1;
    }

    /* Grow cache to cover the requested index if necessary */
    if (n + 1 > cache_len) {
        idx_t new_len = n + 1;
        if (cache_len == 0)
            cache = (seq_t*)malloc(new_len * sizeof(seq_t));
        else
            cache = (seq_t*)realloc(cache, new_len * sizeof(seq_t));

        /* Compute new terms */
        for (idx_t i = cache_len; i <= n; ++i) {
            seq_t val;
            if (i == 0) {
                val = 0;
            } else if (i < 12) {
                val = (seq_t)(i % 10);
            } else {
                seq_t prev = cache[i - 1];
                idx_t j    = second_occ[(int)prev];
                if (j == (idx_t)-1) j = 0;                 /* safety, should not occur */
                seq_t next = cache[j + 1];
                val = (seq_t)((prev + next) % 10);
            }
            cache[i] = val;

            /* Update occurrence tables for the value just written */
            idx_t old_last = last_occ[(int)val];
            second_occ[(int)val] = old_last;
            last_occ[(int)val]   = i;
        }
        cache_len = new_len;
    }

    return cache[n];
}
