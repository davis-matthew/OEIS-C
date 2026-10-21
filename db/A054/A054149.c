/*
 * A054149  T(2n-1,n), array T as in A054144.
 * https://oeis.org/A054149
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 22, u128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Recurrence:
   a(1) = 1
   a(2) = 14
   a(3) = 84
   a(4) = 410
   a(n) = 8*a(n-1) - 20*a(n-2) + 16*a(n-3) - 4*a(n-4)   (n ≥ 5) */

static seq_t *cache   = NULL;   /* memoized values, cache[i] = a(i+1)   */
static idx_t cache_len = 0;     /* number of cached terms                */

seq_t a(idx_t n)
{
    if (n == 0) return 0;                 /* undefined for 0, return 0            */

    if (cache_len < n) {                  /* need to compute more terms            */
        size_t new_size = (size_t)n * sizeof(seq_t);
        seq_t *new_cache = (seq_t*)realloc(cache, new_size);
        if (!new_cache) exit(EXIT_FAILURE);
        cache = new_cache;

        for (idx_t j = cache_len; j < n; ++j) {
            if (j < 4) {                  /* initial four terms                   */
                switch (j) {
                    case 0: cache[j] = (seq_t)1;   break;
                    case 1: cache[j] = (seq_t)14;  break;
                    case 2: cache[j] = (seq_t)84;  break;
                    case 3: cache[j] = (seq_t)410; break;
                }
            } else {
                cache[j] = (seq_t)8   * cache[j-1]
                         - (seq_t)20  * cache[j-2]
                         + (seq_t)16  * cache[j-3]
                         - (seq_t)4   * cache[j-4];
            }
        }
        cache_len = n;
    }
    return cache[n-1];
}
