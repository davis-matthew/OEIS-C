/*
 * A231822  Number of n X 2 0..2 arrays with no element having a strict
 *          majority of its horizontal, vertical and antidiagonal neighbors
 *          equal to itself plus one mod 3, with upper left element zero (rock
 *          paper and scissors drawn positions).
 * https://oeis.org/A231822
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 9*a(n-1) - 16*a(n-2) + 11*a(n-3) - 27*a(n-4) + 21*a(n-5)
 *            - 4*a(n-6) Initial values: a(1)=1, a(2)=9, a(3)=64, a(4)=439,
 *            a(5)=2992, a(6)=20382
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 23, u128 n <= 47,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

static seq_t *cache = NULL;          /* dynamic cache of computed terms */
static size_t cache_len = 0;         /* current allocated size (max index + 1) */
static bool cache_inited = false;    /* flag: base terms inserted? */

/* ensure cache has room for indices 0 .. need (inclusive) */
static void ensure_cache_size(idx_t need)
{
    if ((idx_t)(cache_len) > need) return;
    size_t old_len = cache_len;
    seq_t *new_cache = (seq_t *)realloc(cache, (need + 1) * sizeof(seq_t));
    if (!new_cache) abort();          /* abort on allocation failure */
    /* zero any newly allocated slots */
    if (need >= old_len) {
        memset(new_cache + old_len, 0,
               ((size_t)(need + 1) - old_len) * sizeof(seq_t));
    }
    cache = new_cache;
    cache_len = need + 1;
}

/* insert the known initial terms */
static void init_cache(void)
{
    if (cache_inited) return;
    /* make sure the cache is large enough for indices 0..6 */
    ensure_cache_size(6);
    cache[0] = (seq_t)0;                     /* unused sentinel */
    cache[1] = (seq_t)1;
    cache[2] = (seq_t)9;
    cache[3] = (seq_t)64;
    cache[4] = (seq_t)439;
    cache[5] = (seq_t)2992;
    cache[6] = (seq_t)20382;
    cache_inited = true;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;      /* undefined for n < 1 -> return 0 */

    init_cache();                     /* ensure base terms are present */

    /* make cache large enough for the requested index */
    ensure_cache_size(n);

    if (cache[n] != (seq_t)0)           /* already computed? */
        return cache[n];

    /* compute missing terms up to n */
    for (idx_t i = 7; i <= n; ++i) {
        seq_t v = (seq_t)9 * cache[i-1];
        v -= (seq_t)16 * cache[i-2];
        v += (seq_t)11 * cache[i-3];
        v -= (seq_t)27 * cache[i-4];
        v += (seq_t)21 * cache[i-5];
        v -= (seq_t) 4 * cache[i-6];
        cache[i] = v;
    }

    return cache[n];
}
