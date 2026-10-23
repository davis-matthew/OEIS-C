/*
 * A129804  a(0) = 1, a(1) = 2; for n>0, a(2*n) = 3*a(2*n-1) - a(2*n-2),
 *          a(2*n+1) = 3*a(2*n) - a(2*n-1) - a(n-1).
 * https://oeis.org/A129804
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=1, a(1)=2; a(2*n) = 3*a(2*n-1) - a(2*n-2); a(2*n+1) =
 *            3*a(2*n) - a(2*n-1) - a(n-1) (n>0)
 * Range:     u8 n <= 6, u16 n <= 12, u32 n <= 23, u64 n <= 27, u128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* cache storage for computed terms */
static seq_t *cache = NULL;      /* pointer to array of a(n) */
static idx_t cache_size = 0;     /* number of elements allocated    */

/* ensure that every index 0…n is computed and available */
static void ensure_cache(idx_t n)
{
    if (cache_size > n) return;                     /* already big enough */

    idx_t new_size = n + 1;                         /* new required size   */
    seq_t *tmp;
#ifdef __cplusplus
    tmp = static_cast<seq_t*>(realloc(cache, new_size * sizeof(seq_t)));
#else
    tmp = (seq_t*)realloc(cache, new_size * sizeof(seq_t));
#endif
    if (tmp == NULL) abort();                      /* allocation failure  */
    cache = tmp;

    /* initialise newly allocated cells to 0 (unknown) */
    for (idx_t i = cache_size; i < new_size; ++i) cache[i] = 0;
    cache_size = new_size;

    /* initialise known initial values if not already set */
    if (cache_size > 0 && cache[0] == 0) cache[0] = 1;
    if (cache_size > 1 && cache[1] == 0) cache[1] = 2;

    /* compute any terms that are still unknown */
    for (idx_t i = (cache_size > 2 ? 2 : cache_size); i <= n; ++i) {
        if (cache[i] != 0) continue;               /* already computed */

        if ((i & 1u) == 0) {                       /* even: i = 2*k   */
            cache[i] = (seq_t)3 * cache[i - 1] - cache[i - 2];
        } else {                                   /* odd:  i = 2*k+1 */
            idx_t k = (i - 1) / 2;
            cache[i] = (seq_t)3 * cache[i - 1]
                       - cache[i - 2] - cache[k - 1];
        }
    }
}

seq_t a(idx_t n)
{
    ensure_cache(n);
    return cache[n];
}
