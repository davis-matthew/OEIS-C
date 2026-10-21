/*
 * A084329  a(0)=0, a(1)=1, a(n) = 20*a(n-1) - 20*a(n-2).
 * https://oeis.org/A084329
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=0, a(1)=1, a(n)=20*a(n‑1)-20*a(n‑2)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 16, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>

static seq_t *cache = NULL;
static idx_t cache_len = 0;

/* Ensure cache contains values up to index n */
static void ensure_cache(idx_t n)
{
    if (cache_len > n) return;

    size_t new_len = (size_t)(n + 1);
    seq_t *new_cache = (seq_t *)malloc(new_len * sizeof(seq_t));
    if (!new_cache) return;                     /* allocation failed – caller must handle */

    if (cache_len == 0) {
        new_cache[0] = 0;
        if (new_len > 1) new_cache[1] = 1;
        cache_len = (new_len > 1) ? 2 : 1;
    } else {
        memcpy(new_cache, cache, cache_len * sizeof(seq_t));
    }

    for (idx_t i = cache_len; i <= n; ++i) {
        if (i == 0) {
            new_cache[i] = 0;
        } else if (i == 1) {
            new_cache[i] = 1;
        } else {
            new_cache[i] = 20 * new_cache[i - 1] - 20 * new_cache[i - 2];
        }
    }

    free(cache);
    cache = new_cache;
    cache_len = new_len;
}

seq_t a(idx_t n)
{
    ensure_cache(n);
    return cache[n];
}
