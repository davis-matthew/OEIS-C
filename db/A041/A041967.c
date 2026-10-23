/*
 * A041967  Denominators of continued fraction convergents to sqrt(506).
 * https://oeis.org/A041967
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 19, u128 n <= 39,
 *            big any n
 * Helpers:   alloc_cache, free_cache
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = 90*a(n-2) - a(n-4)
   initial values: a(0)=1, a(1)=2, a(2)=89, a(3)=180
*/
#include <stddef.h>
#include <stdbool.h>
#include "../utils/alloc_cache.h"
#include "../utils/free_cache.h"

static seq_t *cache = NULL;
static idx_t cache_size = 0;

/* Ensure cache can store indices up to n (inclusive). */
static void ensure_cache(idx_t n)
{
    if (n < cache_size) return;

    idx_t new_size = n + 1;
    seq_t *new_cache = alloc_cache(new_size);
    for (idx_t i = 0; i < cache_size; ++i) new_cache[i] = cache[i];
    if (cache) free_cache(cache);
    cache = new_cache;
    cache_size = new_size;
}

seq_t a(idx_t n)
{
    ensure_cache(n);

    if (cache[n] != 0) return cache[n];

    if (n == 0) return cache[0] = (seq_t)1;
    if (n == 1) return cache[1] = (seq_t)2;
    if (n == 2) return cache[2] = (seq_t)89;
    if (n == 3) return cache[3] = (seq_t)180;

    for (idx_t k = 4; k <= n; ++k) {
        if (cache[k] == 0) {
            cache[k] = (seq_t)90 * cache[k-2] - cache[k-4];
        }
    }
    return cache[n];
}
