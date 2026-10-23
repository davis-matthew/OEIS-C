/*
 * A020758  Expansion of 1/((1-6x)(1-10x)(1-12x)).
 * https://oeis.org/A020758
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 22*a(n-1) - 120*a(n-2) + 6^n ; a(0) = 1, a(1) = 28
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 8, u64 n <= 16, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>   /* size_t */

/* static state for memoisation */
static seq_t *cache   = NULL;   /* array of computed terms          */
static size_t cache_size = 0;   /* number of entries actually filled */
static size_t cache_cap  = 0;   /* allocated size of 'cache'        */
static seq_t pow6_last = 0;     /* 6^(cache_size-1), last power of 6 */

/* extend cache to contain all terms up to index n (inclusive) */
static void extend_cache_to(idx_t n)
{
    /* ensure cache array is large enough */
    size_t need = (size_t)(n + 1);
    if (need > cache_cap) {
        size_t new_cap = need * 2;
        if (new_cap < need) new_cap = need;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) exit(1);
        cache = tmp;
        cache_cap = new_cap;
    }

    /* initialisation if cache is empty */
    if (cache_size == 0) {
        cache[0] = (seq_t)1;               /* a(0)   */
        cache_size = 1;
        pow6_last = (seq_t)1;              /* 6^0    */
    }

    /* establish a(1) if not yet present */
    if (cache_size == 1 && n >= 1) {
        cache[1] = (seq_t)28;              /* a(1)   */
        cache_size = 2;
        pow6_last = (seq_t)6;              /* 6^1    */
    }

    /* extend from current cache_size up to n */
    for (idx_t i = cache_size; i <= n; ++i) {
        /* pow6_next = 6^i   (pow6_last currently holds 6^(i-1)) */
        seq_t pow6_next = pow6_last * (seq_t)6;

        /* a(i) = 22*a(i-1) - 120*a(i-2) + 6^i
           compute as (22*a(i-1) + 6^i) - 120*a(i-2) to avoid
           unsigned underflow for the subtraction */
        seq_t term1 = (seq_t)22 * cache[i - 1];
        seq_t term2 = (seq_t)120 * cache[i - 2];
        seq_t val   = term1 + pow6_next - term2;

        cache[i] = val;
        pow6_last = pow6_next;           /* update for next iteration */
    }

    cache_size = (size_t)(n + 1);
}

/* public sequence routine */
seq_t a(idx_t n)
{
    extend_cache_to(n);
    return cache[n];
}
