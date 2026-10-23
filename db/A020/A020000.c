/*
 * A020000  Expansion of 1/((1-5*x)*(1-7*x)*(1-11*x)).
 * https://oeis.org/A020000
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 8, u64 n <= 17, u128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A020000: Expansion of 1/((1-5*x)*(1-7*x)*(1-11*x)).
   Recurrence: a(0)=1, a(1)=23, a(2)=362
                a(n)=23*a(n-1)-167*a(n-2)+385*a(n-3)  for n>2. */
#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache   = NULL;
    static idx_t cache_len = 0;  /* current cache size */

    /* enlarge cache if needed */
    if (cache_len <= n) {
        size_t new_size = (size_t)n + 1;
        seq_t *tmp =
#if defined(__cplusplus)
            static_cast<seq_t*>(realloc(cache, new_size * sizeof(seq_t)));
#else
            realloc(cache, new_size * sizeof(seq_t));
#endif
        if (tmp == NULL) abort();
        cache = tmp;

        for (idx_t i = cache_len; i <= n; ++i) {
            if (i == 0)
                cache[i] = (seq_t)1;
            else if (i == 1)
                cache[i] = (seq_t)23;
            else if (i == 2)
                cache[i] = (seq_t)362;
            else
                cache[i] = (seq_t)23 * cache[i - 1]
                         - (seq_t)167 * cache[i - 2]
                         + (seq_t)385 * cache[i - 3];
        }
        cache_len = new_size;
    }
    return cache[n];
}
