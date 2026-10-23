/*
 * A156595  Fixed point of the morphism 0->011, 1->010.
 * https://oeis.org/A156595
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nice, nonn, changed
 * Formula:   a(0) = 0 For n > 0: if n % 3 == 0 a(n) = 0 else if n % 3 == 1
 *            a(n) = 1 else a(n) = 1 - a(n/3) this recurrence follows from the
 *            morphism 0→011, 1→010
 * Range:     u8 n <= 104, u16 n <= 104, u32 n <= 104, u64 n <= 104,
 *            u128 n <= 104, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for memoisation */
    static seq_t *cache      = NULL;
    static idx_t cache_size  = 0;   /* next index to compute (cache_size == last_index+1) */
    static idx_t cache_cap   = 0;   /* allocated capacity */

    /* grow cache if necessary */
    if (n + 1 > cache_cap) {
        idx_t new_cap = cache_cap ?: 1;
        while (new_cap <= n) new_cap <<= 1;   /* double until large enough */

        seq_t *tmp = (seq_t *)realloc(cache, (size_t)new_cap * sizeof(seq_t));
        if (!tmp) abort();                   /* out of memory: terminate */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* compute values up to n inclusively */
    for (idx_t i = cache_size; i <= n; ++i) {
        if (i == 0) {
            cache[0] = 0;
        } else {
            idx_t r = i % 3;
            if (r == 0) {
                cache[i] = 0;
            } else if (r == 1) {
                cache[i] = 1;
            } else {  /* r == 2 */
                idx_t q = i / 3;
                cache[i] = 1 - cache[q];
            }
        }
    }
    cache_size = n + 1;
    return cache[n];
}
