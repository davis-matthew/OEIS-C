/*
 * A073352  Positive integers making n^2*(n-1)*(2*n-1)^2*(7*n-1)/36 a square.
 * https://oeis.org/A073352
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 17*a(n-1) - 17*a(n-2) + a(n-3) Initial values: a(0)=1,
 *            a(1)=4, a(2)=55 The recurrence follows from the generating
 *            function (1 - 13*x + 4*x^2)/((1-x)*(1-16*x+x^2))
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 16, u128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

static seq_t *cache = NULL;          /* allocated cache for computed terms   */
static idx_t  cache_cap = 0;         /* current capacity of cache           */
static idx_t  cache_len = 0;         /* number of terms actually stored     */

/* Ensure the cache can hold terms up to and including index n. */
static void ensure_cache_up_to(idx_t n)
{
    if (n < cache_len)
        return;                      /* already have it */

    /* grow capacity if necessary */
    if (cache_cap == 0)
        cache_cap = 4;
    while (cache_cap <= n)
        cache_cap <<= 1;              /* double until large enough */

    cache = (seq_t*)realloc(cache, cache_cap * sizeof(seq_t));

    /* Fill in new entries starting from the current length */
    if (cache_len == 0) {
        cache[0] = (seq_t)1;          /* a(0) */
        cache_len = 1;
    }

    while (cache_len <= n) {
        seq_t val;
        /* compute a(1) and a(2) explicitly */
        if (cache_len == 1)
            val = (seq_t)4;
        else if (cache_len == 2)
            val = (seq_t)55;
        else {
            /* recurrence: a(n) = 17*a(n-1) - 17*a(n-2) + a(n-3) */
            seq_t t1 = cache[cache_len-1];
            seq_t t2 = cache[cache_len-2];
            seq_t t3 = cache[cache_len-3];
            val = t1 * (seq_t)17 - t2 * (seq_t)17 + t3;
        }
        cache[cache_len] = val;
        ++cache_len;
    }
}

seq_t a(idx_t n)
{
    ensure_cache_up_to(n);
    return cache[n];
}
