/*
 * A189727  Fixed point of the morphism 0->011, 1->110.
 * https://oeis.org/A189727
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1) = 0 for n >= 1: if n % 3 == 2 -> a(n) = 1 else if n % 3 ==
 *            0 -> a(n) = 1 - a(n/3) else -> a(n) = a((n+2)/3) a(0) = 0 (used
 *            only for safety in recursion)
 * Range:     u8 n <= 122, u16 n <= 122, u32 n <= 122, u64 n <= 122,
 *            u128 n <= 122, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

static idx_t cache_cap = 0;
static seq_t *cache = NULL;
static uint8_t *vis = NULL;

static void ensure_cache(idx_t n)
{
    if (n <= cache_cap) return;
    idx_t new_cap = n;
    if (new_cap < 2) new_cap = 2;

    cache = (seq_t *)realloc(cache, (new_cap + 1) * sizeof(seq_t));
    vis   = (uint8_t *)realloc(vis,   (new_cap + 1) * sizeof(uint8_t));

    /* initialise new visitation flags to 0 */
    for (idx_t i = cache_cap + 1; i <= new_cap; ++i) vis[i] = 0;

    cache_cap = new_cap;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* safety for recursion */

    ensure_cache(n);
    if (vis[n]) return cache[n];

    seq_t val;
    if (n == 1) {
        val = 0;
    } else if (n % 3 == 2) {
        val = 1;
    } else if (n % 3 == 0) {
        idx_t k = n / 3;
        val = 1 - a(k);
    } else { /* n % 3 == 1 */
        idx_t k = (n + 2) / 3;
        val = a(k);
    }

    cache[n] = val;
    vis[n]   = 1;
    return val;
}
