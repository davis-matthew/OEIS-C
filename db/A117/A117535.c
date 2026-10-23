/*
 * A117535  Number of ways of writing n as a sum of powers of 3, each power
 *          being used at most 4 times.
 * https://oeis.org/A117535
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, look, changed
 * Formula:   a(n) = number of ways of writing n as a sum of powers of 3, each
 *            power used at most 4 times. Recurrence: a(0) = a(1) = a(2) = 1
 *            for n ≥ 1, let m = n / 3 and r = n % 3 if r == 2: a(n) = a(m)
 *            else: a(n) = a(m) + a(m-1)
 * Range:     u8 n <= 9839, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* static cache for memoisation */
static seq_t *cache = NULL;
static bool  *marked = NULL;
static size_t cache_cap = 0;

/* ensure cache has space at least up to idx n */
static void ensure_cache(idx_t n)
{
    size_t needed = (size_t)(n + 1);
    if (cache_cap >= needed) return;

    size_t new_cap = cache_cap ? cache_cap * 2 : 1;
    if (new_cap < needed) new_cap = needed;

    seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    bool  *new_marked = (bool *)realloc(marked, new_cap * sizeof(bool));
    if (!new_cache || !new_marked) exit(EXIT_FAILURE);

    /* initialise new positions as uncomputed */
    for (size_t i = cache_cap; i < new_cap; ++i) new_marked[i] = false;

    cache   = new_cache;
    marked  = new_marked;
    cache_cap = new_cap;
}

seq_t a(idx_t n)
{
    if (n <= 2) return 1;               /* base values */

    if (n < cache_cap && marked[n])     /* already computed */
        return cache[n];

    ensure_cache(n);                    /* make room for index n */

    idx_t m = n / 3;
    idx_t r = n % 3;
    seq_t res;

    if (r == 2)                          /* a(3m+2) = a(m) */
        res = a(m);
    else                                 /* a(3m)   = a(m)+a(m-1) */
        res = a(m) + a(m - 1);          /* also covers a(3m+1) */

    cache[n]  = res;
    marked[n] = true;
    return res;
}
