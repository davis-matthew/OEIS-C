/*
 * A208072  Number of 7Xn 0..1 arrays avoiding 0 0 0 and 1 1 1 horizontally
 *          and 0 0 1 and 0 1 1 vertically.
 * https://oeis.org/A208072
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) + 4*a(n-2) + 39*a(n-3) + 20*a(n-4) + 19*a(n-5) -
 *            191*a(n-6) - 42*a(n-7) - 36*a(n-8) + 216*a(n-9) for n > 11;
 *            initial values: a(1)=20, a(2)=400, a(3)=432, a(4)=2688,
 *            a(5)=11424, a(6)=44064, a(7)=198000, a(8)=844800, a(9)=3542544,
 *            a(10)=15213984, a(11)=64859616
 * Range:     u8 n <= 1, u16 n <= 6, u32 n <= 13, u64 n <= 29, u128 n <= 59,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>

/* dynamic cache for computed values; index 0 unused */
static seq_t *cache = NULL;
static idx_t cache_size   = 0; /* highest index computed */
static idx_t cache_cap    = 0; /* allocated capacity */

/* initialise the cache with base values up to n = 11 */
static void init_cache(void)
{
    if (cache != NULL)
        return;
    /* allocate capacity for indices 0..11 (12 slots) */
    cache_cap = 12;
    cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
    /* initialise with 0 for safety */
    for (idx_t i = 0; i < cache_cap; ++i)
        cache[i] = (seq_t)0;
    /* base values; index 0 unused */
    cache[1] = (seq_t)20;
    cache[2] = (seq_t)400;
    cache[3] = (seq_t)432;
    cache[4] = (seq_t)2688;
    cache[5] = (seq_t)11424;
    cache[6] = (seq_t)44064;
    cache[7] = (seq_t)198000;
    cache[8] = (seq_t)844800;
    cache[9] = (seq_t)3542544;
    cache[10] = (seq_t)15213984;
    cache[11] = (seq_t)64859616;
    cache_size = 11;
}

/* ensure the cache can hold up to index n */
static void ensure_capacity(idx_t n)
{
    if (n < cache_cap)
        return;
    idx_t new_cap = n + 1; /* +1 to include index n */
    seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!new_cache) {
        /* failed realloc – aborting; caller will get 0 */
        return;
    }
    /* initialise new slots to 0 */
    for (idx_t i = cache_cap; i < new_cap; ++i)
        new_cache[i] = (seq_t)0;
    cache = new_cache;
    cache_cap = new_cap;
}

/* compute a(n) for n >= 1 */
seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;
    init_cache();
    ensure_capacity(n);
    /* compute values up to n if not already done */
    for (idx_t i = cache_size + 1; i <= n; ++i) {
        if (i <= 11) {
            /* base values already set */
            continue;
        }
        /* recurrence:
           a(i) = a(i-1) + 4*a(i-2) + 39*a(i-3) + 20*a(i-4)
                 + 19*a(i-5) + 216*a(i-9)
                 - 191*a(i-6) - 42*a(i-7) - 36*a(i-8) */
        seq_t val = cache[i-1];
        val += 4 * cache[i-2];
        val += 39 * cache[i-3];
        val += 20 * cache[i-4];
        val += 19 * cache[i-5];
        val += 216 * cache[i-9];
        val -= 191 * cache[i-6];
        val -= 42 * cache[i-7];
        val -= 36 * cache[i-8];
        cache[i] = val;
    }
    cache_size = (cache_size > n) ? cache_size : n;
    return cache[n];
}
