/*
 * A075918  Fifth column of triangle A075501.
 * https://oeis.org/A075918
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 90*a(n-1) - 3060*a(n-2) + 48600*a(n-3) - 355104*a(n-4) +
 *            933120*a(n-5) with a(0)=1, a(1)=90, a(2)=5040, a(3)=226800,
 *            a(4)=9008496
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 12, u128 n <= 14,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include <string.h>

static seq_t *cache = NULL;
static size_t cache_len = 0;
static size_t cache_cap = 0;

seq_t a(idx_t n)
{
    /* Return cached value if possible */
    if ((size_t)n < cache_len) {
        return cache[n];
    }

    /* Expand cache if needed */
    size_t need = (size_t)n + 1;
    if (need > cache_cap) {
        size_t new_cap = need > 1 ? need : 1;
        seq_t *new_arr = (seq_t *)malloc(new_cap * sizeof(seq_t));
        if (!new_arr) {
            /* Allocation failed – abort (undefined behaviour in this context) */
            return (seq_t)0;
        }
        if (cache) {
            memcpy(new_arr, cache, cache_len * sizeof(seq_t));
            free(cache);
        }
        cache = new_arr;
        cache_cap = new_cap;
    }

    /* Fill missing terms up to n */
    for (size_t i = cache_len; i < need; ++i) {
        if (i == 0) {
            cache[i] = (seq_t)1;
        } else if (i == 1) {
            cache[i] = (seq_t)90;
        } else if (i == 2) {
            cache[i] = (seq_t)5040;
        } else if (i == 3) {
            cache[i] = (seq_t)226800;
        } else if (i == 4) {
            cache[i] = (seq_t)9008496;
        } else {
            /* Recurrence of order 5 */
            seq_t v = (seq_t)90 * cache[i - 1];
            v -= (seq_t)3060 * cache[i - 2];
            v += (seq_t)48600 * cache[i - 3];
            v -= (seq_t)355104 * cache[i - 4];
            v += (seq_t)933120 * cache[i - 5];
            cache[i] = v;
        }
    }

    cache_len = need;
    return cache[n];
}
