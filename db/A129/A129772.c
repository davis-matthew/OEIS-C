/*
 * A129772  a(0) = 1, a(1) = 2; for n > 0, a(2n) = 3a(2n-1), a(2n+1) = 3a(2n)
 *          - 2a(n-1).
 * https://oeis.org/A129772
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1, a(1) = 2; for n > 0: a(2n) = 3 * a(2n-1) a(2n+1) = 3 *
 *            a(2n) - 2 * a(n-1)
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 20, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>

static seq_t *cache = NULL;
static idx_t cache_size = 0;

static void
grow_cache(idx_t n)
{
    if (cache_size > n)
        return;
    idx_t new_size = cache_size ? cache_size * 2 : 1;
    if (new_size < n + 1)
        new_size = n + 1;
    seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
    if (!tmp)
        exit(1);
    if (!cache) {
        memset(tmp, 0, new_size * sizeof(seq_t));
    } else if (new_size > cache_size) {
        memset(tmp + cache_size, 0,
               (new_size - cache_size) * sizeof(seq_t));
    }
    cache = tmp;
    cache_size = new_size;
}

seq_t
a(idx_t n)
{
    grow_cache(n);

    /* initialise the fixed origins */
    cache[0] = (seq_t)1;
    if (n >= 1)
        cache[1] = (seq_t)2;

    /* compute values up to n */
    for (idx_t i = 2; i <= n; ++i) {
        if (cache[i] != (seq_t)0)
            continue;
        if ((i & 1) == 0) {          /* even: i = 2m */
            cache[i] = (seq_t)(3 * cache[i - 1]);
        } else {                     /* odd:  i = 2m+1 */
            idx_t m = (i - 1) / 2;
            /* m > 0 because i >= 3 */
            cache[i] = (seq_t)(3 * cache[i - 1] - 2 * cache[m - 1]);
        }
    }

    return cache[n];
}
