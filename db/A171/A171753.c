/*
 * A171753  Expansion of g.f. 1/(1-3*x-x^2/(1-3*x-x^2/(1-3*x))).
 * https://oeis.org/A171753
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(0)=1, a(1)=3, a(2)=10, a(n)=9*a(n-1)-25*a(n-2)+21*a(n-3) for
 *            n≥3 (linear recurrence with order 3)
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 15, u64 n <= 30, u128 n <= 60,
 *            big any n
 * Helpers:   alloc_seq_array, free_seq_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>                      /* for NULL */
#include "../utils/alloc_seq_array.h"    /* alloc_seq_array */
#include "../utils/free_seq_array.h"     /* free_seq_array */

seq_t a(idx_t n)
{
    /* static cache: cache[i] holds a(i) for i < cache_n */
    static seq_t *cache   = NULL;   /* array of cached values   */
    static idx_t  cache_n = 0;      /* number of cached entries */

    /* If the requested term is not yet cached, enlarge cache */
    if (n + 1 > cache_n) {
        /* Keep old size to know where to start computing */
        idx_t old_cache_n = cache_n;

        /* Allocate larger array */
        seq_t *new_cache = alloc_seq_array(n + 1);
        for (idx_t i = 0; i < old_cache_n; ++i)
            new_cache[i] = cache[i];

        /* Free old array and update pointers */
        free_seq_array(cache);
        cache   = new_cache;
        cache_n = n + 1;

        /* Compute missing terms, starting from old_cache_n */
        for (idx_t k = old_cache_n; k <= n; ++k) {
            switch (k) {
                case 0:  cache[0] = (seq_t)1;    break;
                case 1:  cache[1] = (seq_t)3;    break;
                case 2:  cache[2] = (seq_t)10;   break;
                default:
                    cache[k] = (seq_t)9   * cache[k-1]
                              - (seq_t)25  * cache[k-2]
                              + (seq_t)21  * cache[k-3];
                    break;
            }
        }
    }

    return cache[n];
}
