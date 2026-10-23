/*
 * A040051  Parity of partition function A000041.
 * https://oeis.org/A040051
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(n) = parity of the partition function p(n). Recurrence
 *            (pentagonal theorem) : a(0) = 1, and for n > 0 a(n) =
 *            ⨁_{k=1}^{∞} [a(n - k(3k-1)/2) ⊕ a(n - k(3k+1)/2)], where terms
 *            with negative index are omitted
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for memoisation across calls */
    static seq_t *cache = NULL;      /* array storing a(0)..a(current_max) */
    static idx_t cache_size = 0;     /* how many entries are currently cached */

    /* initialise cache if first call */
    if (cache_size == 0) {
        cache_size = 1;
        cache = (seq_t *)malloc(sizeof(seq_t) * cache_size);
        if (!cache) return 0;                 /* allocation failure – return 0 */
        cache[0] = 1;                         /* a(0) = 1 */
    }

    /* extend cache if we need a larger n */
    if (n >= cache_size) {
        idx_t new_size = n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!tmp) return 0;                   /* allocation failure – return 0 */
        cache = tmp;

        /* compute new entries from old_max+1 up to n */
        for (idx_t i = cache_size; i <= n; ++i) {
            seq_t val = 0;
            for (idx_t k = 1; ; ++k) {
                idx_t g1 = k * (3 * k - 1) / 2;
                if (g1 > i) break;            /* no further pentagonal numbers */
                val ^= cache[i - g1];

                idx_t g2 = k * (3 * k + 1) / 2;
                if (g2 <= i) val ^= cache[i - g2];
            }
            cache[i] = val & 1;                /* keep only parity */
        }
        cache_size = new_size;
    }

    return cache[n] & 1;                      /* return parity of p(n) */
}
